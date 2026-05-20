const state = {
  data: null,
  nodes: [],
  routeLayouts: [],
  running: true,
  selectedRoute: null,
  startedAt: performance.now()
};

const colors = [
  "#0f8b8d", "#e4572e", "#2f9e44", "#7b2cbf", "#f59f00",
  "#1c7ed6", "#c2255c", "#5c940d", "#364fc7", "#087f5b"
];

const canvas = document.querySelector("#graph");
const ctx = canvas.getContext("2d");
const form = document.querySelector("#solver-form");
const instanceEl = document.querySelector("#instance");
const alphaEl = document.querySelector("#alpha");
const alphaValueEl = document.querySelector("#alpha-value");
const iterationsEl = document.querySelector("#iterations");
const emptyState = document.querySelector("#empty-state");
const solveBtn = document.querySelector("#solve");
const titleEl = document.querySelector("#title");
const subtitleEl = document.querySelector("#subtitle");
const routesEl = document.querySelector("#routes");
const loadsEl = document.querySelector("#loads");
const loadPanelEl = document.querySelector("#load-panel");
const loadTitleEl = document.querySelector("#load-title");
const toggleAnimationEl = document.querySelector("#toggle-animation");

function resizeCanvas() {
  if (state.data) {
    updateCanvasHeight(state.data);
  }

  const rect = canvas.getBoundingClientRect();
  const ratio = window.devicePixelRatio || 1;
  canvas.width = Math.max(600, Math.floor(rect.width * ratio));
  canvas.height = Math.max(420, Math.floor(rect.height * ratio));
  ctx.setTransform(ratio, 0, 0, ratio, 0, 0);
  if (state.data) {
    layoutNodes();
  }
}

function routeRowCounts(data, routeWidth) {
  const maxColumns = maxRouteColumns(routeWidth);
  return data.rotas.map((route) => Math.max(1, Math.ceil(route.caminho.length / maxColumns)));
}

function maxRouteColumns(routeWidth) {
  return Math.max(8, Math.floor(routeWidth / 46));
}

function updateCanvasHeight(data) {
  if (!data) {
    canvas.style.height = "";
    return;
  }

  const wrap = canvas.parentElement.getBoundingClientRect();
  const routeWidth = Math.min(wrap.width * 0.78, 900);
  const rows = routeRowCounts(data, routeWidth).reduce((sum, count) => sum + count, 0);
  const required = Math.max(wrap.height, 122 + rows * 46 + Math.max(0, data.rotas.length - 1) * 26);
  canvas.style.height = `${required}px`;
}

function formatNumber(value) {
  return new Intl.NumberFormat("pt-BR").format(value);
}

function formatPercent(value) {
  return `${new Intl.NumberFormat("pt-BR", {
    minimumFractionDigits: 2,
    maximumFractionDigits: 2
  }).format(value)}%`;
}

async function loadInstances() {
  const res = await fetch("/api/instances");
  const data = await res.json();
  instanceEl.innerHTML = data.instancias
    .map((name) => `<option value="${name}">${name}</option>`)
    .join("");
}

function shortestDistance(custos, a, b) {
  const direct = Math.min(custos[a][b], custos[b][a]);
  return direct > 0 ? direct : Math.max(custos[a][b], custos[b][a], 1);
}

function routeInitialLoad(route, demandas, capacity) {
  let accumulated = 0;
  let minLoad = 0;
  let maxLoad = 0;

  route.caminho.forEach((station) => {
    accumulated += demandas[station];
    minLoad = Math.min(minLoad, accumulated);
    maxLoad = Math.max(maxLoad, accumulated);
  });

  const initial = Math.max(0, -minLoad);
  return Math.min(capacity, Math.max(initial, maxLoad > capacity ? capacity - maxLoad : initial));
}

function routeLoadSteps(route) {
  const initial = routeInitialLoad(route, state.data.demandas, state.data.Q);
  let load = initial;

  const steps = [{
    station: 0,
    delta: initial,
    load,
    label: "saida"
  }];

  route.caminho.slice(1, -1).forEach((station) => {
    const delta = state.data.demandas[station];
    load += delta;
    steps.push({
      station,
      delta,
      load,
      label: delta >= 0 ? "coleta" : "entrega"
    });
  });

  steps.push({
    station: 0,
    delta: 0,
    load,
    label: "retorno"
  });

  return steps;
}

function layoutNodes() {
  const data = state.data;
  updateCanvasHeight(data);

  const rect = canvas.getBoundingClientRect();
  const width = Math.max(rect.width, 600);
  const height = Math.max(rect.height, 420);
  const routeWidth = Math.min(width * 0.78, 900);
  const left = Math.max(78, (width - routeWidth) / 2);
  const right = left + routeWidth;
  const top = 52;
  const bottom = Math.max(top + 180, height - 52);
  const rowCounts = routeRowCounts(data, routeWidth);
  const totalRows = rowCounts.reduce((sum, count) => sum + count, 0);
  const routeGap = 26;
  const rowGap = 46;

  state.routeLayouts = data.rotas.map((route, routeIndex) => {
    const rowsBefore = rowCounts.slice(0, routeIndex).reduce((sum, count) => sum + count, 0);
    const yBase = top + rowsBefore * rowGap + routeIndex * routeGap;
    const maxColumns = maxRouteColumns(routeWidth);
    const columns = Math.min(maxColumns, route.caminho.length);
    const step = (right - left) / Math.max(1, columns - 1);

    return route.caminho.map((station, stationIndex) => {
      const row = Math.floor(stationIndex / maxColumns);
      const col = stationIndex % maxColumns;
      const rowStart = row * maxColumns;
      const remaining = route.caminho.length - rowStart;
      const rowSize = Math.min(maxColumns, remaining);
      const rowStep = rowSize === 1 ? 0 : (right - left) / (rowSize - 1);
      const evenRow = row % 2 === 0;
      const x = rowSize === 1
        ? (evenRow ? left : right)
        : evenRow
          ? left + rowStep * col
          : right - rowStep * col;

      return {
        id: station,
        x,
        y: yBase + row * rowGap,
        demand: data.demandas[station],
        depot: station === 0,
        routeIndex,
        stationIndex
      };
    });
  });

  state.nodes = state.routeLayouts.flat();
}

function renderRoutesList() {
  const data = state.data;
  routesEl.innerHTML = data.rotas.map((route, index) => {
    const color = colors[index % colors.length];
    const selected = state.selectedRoute === index ? " selected" : "";
    return `
      <button class="route-item${selected}" type="button" data-route-index="${index}">
        <div class="route-head">
          <strong><span class="route-color" style="background:${color}"></span>Veiculo ${index + 1}</strong>
          <span>${formatNumber(route.custo)}</span>
        </div>
        <div class="route-meta">
          <span>${route.caminho.length - 2} paradas</span>
          <span>saida com ${routeInitialLoad(route, data.demandas, data.Q)}/${data.Q}</span>
        </div>
        <div class="route-path">${route.caminho.join(" -> ")}</div>
      </button>
    `;
  }).join("");
}

function renderLoadPanel(routeIndex) {
  const data = state.data;
  if (routeIndex === null || routeIndex === undefined || !data.rotas[routeIndex]) {
    loadPanelEl.classList.add("hidden");
    loadsEl.innerHTML = "<span>Selecione uma rota para ver a evolucao da carga.</span>";
    return;
  }

  const route = data.rotas[routeIndex];
  const color = colors[routeIndex % colors.length];
  const steps = routeLoadSteps(route);
  const rows = steps.map((step) => {
    const width = Math.max(2, Math.min(100, (step.load / data.Q) * 100));
    const delta = step.station === 0 && step.label === "saida"
      ? `inicial ${step.load}`
      : step.delta > 0
        ? `+${step.delta}`
        : `${step.delta}`;
    return `
      <div class="load-step">
        <span class="load-station">${step.station === 0 ? "Dep." : `E${step.station}`}</span>
        <span class="load-bar">
          <span class="load-fill" style="width:${width}%;background:${color}"></span>
        </span>
        <span class="load-value">${delta} | ${step.load}</span>
      </div>
    `;
  }).join("");

  loadTitleEl.textContent = `Carga do veiculo ${routeIndex + 1}`;
  loadsEl.innerHTML = `
    <article class="load-route">
      <strong><span class="route-color" style="background:${color}"></span>${route.caminho.join(" -> ")}</strong>
      <div class="load-track">${rows}</div>
    </article>
  `;
  loadPanelEl.classList.remove("hidden");
}

function updateMetrics() {
  const data = state.data;
  document.querySelector("#metric-cost").textContent = formatNumber(data.custo);
  if (data.referencia) {
    document.querySelector("#metric-reference").textContent = `${data.referencia.tipo} ${formatNumber(data.referencia.valor)}`;
    document.querySelector("#metric-gap").textContent = formatPercent(data.referencia.gapPercent);
  }
  else {
    document.querySelector("#metric-reference").textContent = "-";
    document.querySelector("#metric-gap").textContent = "-";
  }
  document.querySelector("#metric-routes").textContent = data.rotas.length;
  document.querySelector("#metric-time").textContent = `${data.tempoMs} ms`;
  titleEl.textContent = data.instancia;
  subtitleEl.textContent = `${data.n} estacoes, ${data.m} veiculos, capacidade ${data.Q}`;
}

async function solve(event) {
  event.preventDefault();
  solveBtn.disabled = true;
  solveBtn.textContent = "Resolvendo";

  const params = new URLSearchParams({
    instance: instanceEl.value,
    alpha: alphaEl.value,
    iterations: iterationsEl.value
  });

  try {
    const res = await fetch(`/api/solve?${params}`);
    const data = await res.json();
    if (!res.ok) throw new Error(data.erro || "Erro ao resolver.");

    state.data = data;
    state.selectedRoute = null;
    state.startedAt = performance.now();
    emptyState.classList.add("hidden");
    resizeCanvas();
    renderRoutesList();
    renderLoadPanel(null);
    updateMetrics();
  } catch (err) {
    titleEl.textContent = "Erro ao resolver";
    subtitleEl.textContent = err.message;
  } finally {
    solveBtn.disabled = false;
    solveBtn.textContent = "Resolver";
  }
}

function drawEdge(a, b, color, width, alpha) {
  ctx.save();
  ctx.globalAlpha = alpha;
  ctx.strokeStyle = color;
  ctx.lineWidth = width;
  ctx.lineCap = "round";
  ctx.lineJoin = "round";
  ctx.beginPath();
  ctx.moveTo(a.x, a.y);
  if (Math.abs(a.y - b.y) > 1) {
    const midY = (a.y + b.y) / 2;
    ctx.bezierCurveTo(a.x, midY, b.x, midY, b.x, b.y);
  }
  else {
    ctx.lineTo(b.x, b.y);
  }
  ctx.stroke();
  ctx.restore();
}

function drawNode(node) {
  const demand = node.demand;
  const compact = state.data && state.data.rotas.length > 8;
  const radius = node.depot ? (compact ? 12 : 15) : (compact ? 8 : 10);
  const fill = node.depot ? "#17212b" : demand >= 0 ? "#e8f6ef" : "#fff0f5";
  const stroke = node.depot ? "#17212b" : demand >= 0 ? "#2f9e44" : "#d6336c";

  ctx.fillStyle = fill;
  ctx.strokeStyle = stroke;
  ctx.lineWidth = 2;
  ctx.beginPath();
  ctx.arc(node.x, node.y, radius, 0, Math.PI * 2);
  ctx.fill();
  ctx.stroke();

  ctx.fillStyle = node.depot ? "#fff" : "#17212b";
  ctx.font = compact ? "700 9px system-ui" : node.depot ? "700 11px system-ui" : "700 10px system-ui";
  ctx.textAlign = "center";
  ctx.textBaseline = "middle";
  ctx.fillText(node.depot ? "D" : node.id, node.x, node.y);

  if (!node.depot && !compact) {
    ctx.fillStyle = "#667381";
    ctx.font = "11px system-ui";
    ctx.fillText(demand > 0 ? `+${demand}` : demand, node.x, node.y + 23);
  }
}

function routePoint(route, progress, routeIndex, totalRoutes) {
  const layout = state.routeLayouts[routeIndex];
  if (!layout || layout.length === 0) {
    return { x: 0, y: 0, segment: 0, local: 0 };
  }

  if (progress <= 0) {
    return { ...layout[0], segment: 0, local: 0 };
  }
  if (progress >= 1) {
    return { ...layout[layout.length - 1], segment: layout.length - 2, local: 1 };
  }

  const segment = Math.floor(progress * (layout.length - 1));
  const local = progress * (layout.length - 1) - segment;
  const a = layout[Math.min(segment, layout.length - 2)];
  const b = layout[Math.min(segment + 1, layout.length - 1)];

  return {
    x: a.x + (b.x - a.x) * local,
    y: a.y + (b.y - a.y) * local,
    segment,
    local
  };
}

function routeLoadAtProgress(route, progress) {
  const steps = routeLoadSteps(route);
  const segment = Math.floor(progress * (route.caminho.length - 1));
  const stepIndex = Math.min(segment, steps.length - 1);
  return steps[stepIndex].load;
}

function routeAnimationState(route, index, now) {
  const segments = Math.max(1, route.caminho.length - 1);
  const travelMs = Math.max(6500, segments * 700);
  const staggerMs = Math.min(3000, index * 450);
  const depotHoldMs = 1600;
  const cycleMs = staggerMs + travelMs + depotHoldMs;
  const elapsed = (now - state.startedAt) % cycleMs;

  if (elapsed < staggerMs) {
    return { progress: 0, atDepot: true, phase: "saida" };
  }

  if (elapsed >= staggerMs + travelMs) {
    return { progress: 1, atDepot: true, phase: "retorno" };
  }

  return {
    progress: (elapsed - staggerMs) / travelMs,
    atDepot: false,
    phase: "rota"
  };
}

function roundedRectPath(x, y, width, height, radius) {
  const r = Math.min(radius, width / 2, height / 2);
  ctx.moveTo(x + r, y);
  ctx.lineTo(x + width - r, y);
  ctx.quadraticCurveTo(x + width, y, x + width, y + r);
  ctx.lineTo(x + width, y + height - r);
  ctx.quadraticCurveTo(x + width, y + height, x + width - r, y + height);
  ctx.lineTo(x + r, y + height);
  ctx.quadraticCurveTo(x, y + height, x, y + height - r);
  ctx.lineTo(x, y + r);
  ctx.quadraticCurveTo(x, y, x + r, y);
}

function drawVehicle(point, color, load, capacity, index, phase) {
  ctx.save();
  ctx.fillStyle = "#ffffff";
  ctx.strokeStyle = color;
  ctx.lineWidth = 3;
  ctx.beginPath();
  ctx.arc(point.x, point.y, 9, 0, Math.PI * 2);
  ctx.fill();
  ctx.stroke();

  ctx.fillStyle = color;
  ctx.beginPath();
  ctx.arc(point.x, point.y, 4, 0, Math.PI * 2);
  ctx.fill();

  const label = phase === "retorno"
    ? `V${index + 1} fim ${load}/${capacity}`
    : phase === "saida"
      ? `V${index + 1} sai ${load}/${capacity}`
      : `V${index + 1} ${load}/${capacity}`;
  ctx.font = "700 12px system-ui";
  const width = ctx.measureText(label).width + 14;
  const x = point.x + 12;
  const y = point.y - 28;
  ctx.fillStyle = "rgba(255,255,255,0.94)";
  ctx.strokeStyle = "rgba(23,33,43,0.12)";
  ctx.lineWidth = 1;
  ctx.beginPath();
  roundedRectPath(x, y, width, 24, 6);
  ctx.fill();
  ctx.stroke();
  ctx.fillStyle = "#17212b";
  ctx.textAlign = "left";
  ctx.textBaseline = "middle";
  ctx.fillText(label, x + 7, y + 12);
  ctx.restore();
}

function drawRouteLabel(routeIndex, color) {
  const layout = state.routeLayouts[routeIndex];
  if (!layout || layout.length === 0) return;

  const first = layout[0];
  const label = `V${routeIndex + 1}`;

  ctx.save();
  ctx.font = "700 12px system-ui";
  ctx.textAlign = "right";
  ctx.textBaseline = "middle";
  ctx.fillStyle = color;
  ctx.fillText(label, first.x - 24, first.y);
  ctx.restore();
}

function render(now) {
  const rect = canvas.getBoundingClientRect();
  ctx.clearRect(0, 0, rect.width, rect.height);

  if (state.data && state.routeLayouts.length) {
    const totalRoutes = state.data.rotas.length;
    state.routeLayouts.forEach((layout, index) => {
      const color = colors[index % colors.length];
      drawRouteLabel(index, color);
      for (let i = 0; i < layout.length - 1; i++) {
        drawEdge(layout[i], layout[i + 1], color, totalRoutes > 8 ? 2 : 3, totalRoutes > 8 ? 0.28 : 0.42);
      }
    });

    state.routeLayouts.forEach((layout) => {
      layout.forEach(drawNode);
    });

    if (state.running) {
      state.data.rotas.forEach((route, index) => {
        const animation = routeAnimationState(route, index, now);
        const p = routePoint(route, animation.progress, index, totalRoutes);
        const load = routeLoadAtProgress(route, animation.progress);
        drawVehicle(p, colors[index % colors.length], load, state.data.Q, index, animation.phase);
      });
    }
  }

  requestAnimationFrame(render);
}

alphaEl.addEventListener("input", () => {
  alphaValueEl.textContent = Number(alphaEl.value).toFixed(2);
});

form.addEventListener("submit", solve);

routesEl.addEventListener("click", (event) => {
  const item = event.target.closest("[data-route-index]");
  if (!item || !state.data) return;

  const index = Number(item.dataset.routeIndex);
  state.selectedRoute = state.selectedRoute === index ? null : index;
  renderRoutesList();
  renderLoadPanel(state.selectedRoute);
});

toggleAnimationEl.addEventListener("click", () => {
  state.running = !state.running;
  toggleAnimationEl.textContent = state.running ? "Pausar" : "Animar";
  toggleAnimationEl.setAttribute("aria-pressed", String(state.running));
});

document.querySelector("#fit").addEventListener("click", () => {
  if (state.data) {
    layoutNodes();
    state.startedAt = performance.now();
  }
});

window.addEventListener("resize", resizeCanvas);

resizeCanvas();
loadInstances();
requestAnimationFrame(render);
