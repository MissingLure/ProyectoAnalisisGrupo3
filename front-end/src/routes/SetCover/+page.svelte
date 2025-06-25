<script lang="ts">
	import type { PageData } from './$types';
	import axios from 'axios';
	import { goto } from '$app/navigation';

	type GrupoSet = {
		indice: number;
		conjunto: number[];
	};

	let gruposGenerados: string[] = []; // Simulación para mostrar grupos
	let tiempoMs: number | null = null;
	let totalSeleccionados: number | null = null;
	let usarExacto = false; // false = Aproximado, true = Exacto

	let clases = 0;
	let secciones = 0;
	let minPorSeccion = 0;
	let maxPorSeccion = 0;

	let errores: string[] = [];

	async function validarYGenerar() {
		errores = []; // Limpiamos errores anteriores

		if (clases <= 0) errores.push('La cantidad de clases debe ser mayor a 0.');
		if (secciones <= 0) errores.push('La cantidad de secciones debe ser mayor  a 0.');
		if (minPorSeccion <= 0) errores.push('El mínimo de alumnos por sección debe ser mayor a 0.');
		if (maxPorSeccion <= 0) errores.push('El máximo de alumnos por sección debe ser mayor a 0.');
		if (minPorSeccion > maxPorSeccion) errores.push('El mínimo no puede ser mayor que el máximo.');
		if (secciones * minPorSeccion < clases) {
			errores.push(
				'La cantidad mínima total de clases no cubre el universo. Aumenta el número de secciones o el mínimo por sección.'
			);
		}

		if (errores.length > 0) return;

		try {
			const endpoint = usarExacto
				? '/api/SetCoverExacto'
				: '/api/SetCoverAproximado';

			const response = await axios.post(endpoint, {
				numSets: secciones,
				minSetSize: minPorSeccion,
				maxSetSize: maxPorSeccion,
				universeSize: clases
			});

			console.log('Respuesta de la API:', response.data);

			tiempoMs = response.data.tiempo_ms;
			totalSeleccionados = response.data.num_conjuntos;
			const solucion = response.data.solucion as GrupoSet[];

			gruposGenerados = solucion.map((grupo, i) => {
				return `${grupo.conjunto.join(', ')}`;
			});
		} catch (error) {
			console.error('Error al conectar con el backend:', error);
			errores.push('No se pudo conectar al servidor.');
		}
	}

	 function volverAlMenu() {
    goto('/Menu'); // Cambiá a '/menu' si renombrás
  }
</script>

<svelte:head>
	<link
		href="https://fonts.googleapis.com/css2?family=Permanent+Marker&display=swap"
		rel="stylesheet"
	/>
</svelte:head>

<main
	class="flex min-h-screen flex-col items-start gap-10 bg-gradient-to-br from-indigo-100 via-white to-cyan-100 px-4 py-12 md:flex-row md:items-start"
>
<button
  on:click={volverAlMenu}
  class="mt-8 bg-white text-indigo-700 font-semibold py-2 px-6 rounded-full shadow-md hover:bg-indigo-200 transition"
>
  ⬅ Volver al Menú
</button>
	<!-- Panel de entrada -->
	<div
		class="w-full max-w-md overflow-y-auto rounded-3xl border border-indigo-200 bg-white p-8 shadow-2xl"
	>
		<h1
			class="mb-6 text-center text-5xl text-black drop-shadow-[2px_2px_1px_rgba(0,0,0,0.3)]"
			style="font-family: 'Permanent Marker', cursive;"
		>
			Set Cover Académico
		</h1>

		<!-- Inputs -->
		<!-- Clases -->
		<div>
			<label for="clases" class="mb-1 block font-semibold text-indigo-800">
				Clases de Ingeniería en Sistemas Computacionales
			</label>

			<input
				id="clases"
				type="number"
				bind:value={clases}
				class="w-full rounded-lg border border-gray-300 px-4 py-2 focus:outline-none focus:ring-2 focus:ring-indigo-400"
			/>
		</div>

		<!-- Secciones -->
		<!-- Cantidad de secciones -->
		<div>
			<label for="secciones" class="mb-1 block font-semibold text-indigo-800">
				Cantidad de secciones
			</label>
			<input
				id="secciones"
				type="number"
				bind:value={secciones}
				class="w-full rounded-lg border border-gray-300 px-4 py-2 focus:outline-none focus:ring-2 focus:ring-indigo-400"
				placeholder="Ej: 5"
			/>
		</div>

		<!-- Mínimo por sección -->
		<div>
			<label for="minPorSeccion" class="mb-1 block font-semibold text-indigo-800">
				Mínimo de alumnos por sección
			</label>
			<input
				id="minPorSeccion"
				type="number"
				bind:value={minPorSeccion}
				class="w-full rounded-lg border border-gray-300 px-4 py-2 focus:outline-none focus:ring-2 focus:ring-indigo-400"
				placeholder="Ej: 3"
			/>
		</div>

		<!-- Máximo por sección -->
		<div>
			<label for="maxPorSeccion" class="mb-1 block font-semibold text-indigo-800">
				Máximo de alumnos por sección
			</label>
			<input
				id="maxPorSeccion"
				type="number"
				bind:value={maxPorSeccion}
				class="w-full rounded-lg border border-gray-300 px-4 py-2 focus:outline-none focus:ring-2 focus:ring-indigo-400"
				placeholder="Ej: 8"
			/>
		</div>

		<!-- Botón -->
		<!-- Botón + Toggle -->
		<div class="mt-8 flex flex-wrap items-center justify-between gap-4">
			<!-- Botón -->
			<button
				class="rounded-xl bg-indigo-600 px-6 py-3 text-white shadow-md transition hover:bg-indigo-700"
				on:click={() => {
					console.log({ clases, secciones, minPorSeccion, maxPorSeccion });
					validarYGenerar();
				}}
			>
				Generar Grupos
			</button>

			<!-- Toggle Aproximado/Exacto -->
			<div class="flex items-center gap-2">
				<span class="text-sm font-medium text-indigo-800">
					{usarExacto ? '🧠 Exacto' : '⚡ Aproximado'}
				</span>
				<label class="relative inline-flex cursor-pointer items-center">
					<input type="checkbox" class="peer sr-only" bind:checked={usarExacto} />
					<div
						class="peer h-7 w-14 rounded-full bg-gray-300 transition peer-checked:bg-indigo-600 peer-focus:outline-none"
					></div>
					<span
						class="absolute left-1 top-1 h-5 w-5 rounded-full bg-white transition peer-checked:translate-x-7"
					></span>
				</label>
			</div>
		</div>

		{#if errores.length > 0}
			<div class="mt-6 space-y-2 text-sm text-red-600">
				{#each errores as error}
					<p>❌ {error}</p>
				{/each}
			</div>
		{/if}

		<!-- Resultados (centro de pantalla) -->
	</div>
	<div
		class="flex-1 self-stretch rounded-2xl border border-gray-300 bg-white/60 p-8 text-gray-700 shadow-lg"
	>
		{#if gruposGenerados.length === 0}
			<p class="w-full text-center text-lg font-medium">No hay ningún grupo aún.</p>
		{:else}
			<!-- Info adicional -->
			<div class="mb-4 space-y-2">
				<p class="text-base font-semibold text-indigo-700">
					⏱️ Tiempo de ejecución: {tiempoMs?.toFixed(2)} ms
				</p>
				<p class="text-base font-semibold text-indigo-700">
					📦 Grupos seleccionados: {totalSeleccionados}
				</p>
			</div>

			<!-- Lista de grupos -->
			<ul class="w-full space-y-2">
				{#each gruposGenerados as grupo, i}
					<li class="rounded-lg bg-white px-4 py-2 shadow">
						<strong>Grupo {i + 1}:</strong>
						{grupo}
					</li>
				{/each}
			</ul>
		{/if}
	</div>
</main>
