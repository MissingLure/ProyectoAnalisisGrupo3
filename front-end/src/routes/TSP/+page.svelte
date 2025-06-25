<script lang="ts">
	//imports para el mapa
	import { onMount } from 'svelte';
	import type * as L from 'leaflet';
	import 'leaflet/dist/leaflet.css';
	import axios from 'axios';
	import postTSP from './TSP';
	import { goto } from '$app/navigation';

	const MAX_DOTS = 15;

	let cant_dots = 3; //cantidad de marcas
	let mapa: L.Map; //Para el mapa en si
	let dots: L.CircleMarker[] = []; //Lista de marcas
	let pathLine: L.Polyline | null = null;

	// bordes del mapa
	let southwest: L.LatLng;
	let northeast: L.LatLng;
	let bounds: L.LatLngBounds;

	let tiempo: number | null = null;
	let camino: { lat: number; lng: number }[] = [];
	let distanciaTotal: number | null = null;
	let ruta: number[] = [];
	let rutaText: string = '';
	let opSelected = 'Aproximado';

	// Calcular las distancias proceso y codigo investigado
	function haversineDistance(lat1: number, lon1: number, lat2: number, lon2: number): number {
		const R = 6371e3;
		const grados1 = (lat1 * Math.PI) / 180;
		const grados2 = (lat2 * Math.PI) / 180;
		const difLat = ((lat2 - lat1) * Math.PI) / 180;
		const difLon = ((lon2 - lon1) * Math.PI) / 180;

		const a =
			Math.sin(difLat / 2) * Math.sin(difLat / 2) +
			Math.cos(grados1) * Math.cos(grados2) * Math.sin(difLon / 2) * Math.sin(difLon / 2);
		const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));

		const d = R * c;
		return d;
	}

	// inicializa la matriz de adyaciencia
	function createAdjacencyMatrix(points: { lat: number; lng: number }[]): number[][] {
		const n = points.length;
		const matriz: number[][] = Array(n)
			.fill(0)
			.map(() => Array(n).fill(0));

		for (let i = 0; i < n; i++) {
			for (let j = 0; j < n; j++) {
				if (i === j) {
					matriz[i][j] = -1;
				} else {
					matriz[i][j] = haversineDistance(
						points[i].lat,
						points[i].lng,
						points[j].lat,
						points[j].lng
					);
				}
			}
		}
		return matriz;
	}

	function generateDots(count: number) {
		if (!mapa) return;

		//para limpar el mapa
		dots.forEach((x) => mapa.removeLayer(x)); //quita los puntitos
		dots = [];
		if (pathLine) {
			//quita la linea del camino
			mapa.removeLayer(pathLine);
			pathLine = null;
		}

		const generatedPoints: { lat: number; lng: number }[] = [];

		for (let i = 0; i < count; i++) {
			// puntos aleatorios dentro de los bordes
			const lat = southwest.lat + Math.random() * (northeast.lat - southwest.lat);
			const lng = southwest.lng + Math.random() * (northeast.lng - southwest.lng);
			generatedPoints.push({ lat, lng });
			let marker: L.CircleMarker;
			if (i == 0) {
				marker = L.circleMarker([lat, lng], {
					radius: 6,
					color: 'Red',
					fillColor: 'Red',
					fillOpacity: 0.5
				}).addTo(mapa);
			} else {
				//Agrega los puntos al mapa
				marker = L.circleMarker([lat, lng], {
					radius: 6,
					color: 'green',
					fillColor: '#7ca982',
					fillOpacity: 0.5
				}).addTo(mapa);
			}

			//ID del punto solo para verlo en el mapa
			marker.bindPopup(`Punto ${i}`);
			dots.push(marker);
		}

		const matriz = createAdjacencyMatrix(generatedPoints);
		console.log('Matriz de adyacencia:', matriz);
		TSP_Backend(generatedPoints, matriz);
	}

	async function TSP_Backend(points: { lat: number; lng: number }[], matriz: number[][]) {
		try {
			const response = await axios.post('/solve-tsp', { points, matriz });

			const data = response.data;
			console.log('Received data from backend:', data);
			tiempo = data.time;
			distanciaTotal = data.distance;
			camino = data.path;
			ruta = data.path_indices;
			rutaText = '';

			if (mapa && camino.length > 1) {
				if (pathLine) {
					mapa.removeLayer(pathLine);
				}

				const latlngs = camino.map((p) => L.latLng(p.lat, p.lng));
				latlngs.push(latlngs[0]);
				pathLine = L.polyline(latlngs, { color: 'red', weight: 3 }).addTo(mapa);

				for (let i = 0; i < dots.length; i++) {
					dots[i].bindPopup(`Punto ${i} - Peso: ${Math.round(matriz[ruta[i]][ruta[i + 1]])}`);
					rutaText += `${ruta[i]} -> `;
				}
				rutaText += `0`;
			}
		} catch (error) {
			console.error('Error:', error);
			tiempo = null;
			camino = [];
			distanciaTotal = null;

			if (pathLine) {
				mapa.removeLayer(pathLine);
				pathLine = null;
			}
		}
	}

	onMount(async () => {
		const Leaflet = await import('leaflet');
		L = Leaflet;

		southwest = L.latLng(15.562056678278814, -88.02435510950794);
		northeast = L.latLng(15.563675631953194, -88.02273615583356);
		bounds = L.latLngBounds(southwest, northeast);

		mapa = L.map('mapa', {
			maxBounds: bounds,
			maxBoundsViscosity: 1.0,
			minZoom: 18,
			maxZoom: 18
		});

		mapa.fitBounds(bounds);

		L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
			attribution: '&copy; OpenStreetMap contributors'
		}).addTo(mapa);
	});

	//Para cambiar los puntos
	$: {
		//Asegura el maximo y el minimo
		if (cant_dots > MAX_DOTS) {
			cant_dots = MAX_DOTS;
		} else if (cant_dots < 3) {
			cant_dots = 3;
		}
	}

	 function volverAlMenu() {
    goto('/Menu'); // Cambiá a '/menu' si renombrás
    }
</script>

<div
	class="flex min-h-screen flex-col items-center bg-gradient-to-br from-[#e0eec6] to-[#7ca982] p-8 text-black"
>
	<h1 class="titulo">Problema del Viajante de Comercio (TSP)</h1>
	<div
		class="flex w-full max-w-6xl flex-col items-center justify-center gap-8 md:flex-row md:items-start"
	>
		<div id="mapa"></div>
		<div
			class="flex w-full flex-shrink-0 flex-grow-0 flex-col items-center justify-center gap-6 rounded-lg
                    bg-[#D9F4CD] p-8 shadow-xl md:w-80 lg:w-96"
		>
			<div class="flex w-full items-center">
				<label class="mb-2 text-lg font-medium">Cantidad de Nodos:</label>
				<input
					type="number"
					id="cant_dots"
					bind:value={cant_dots}
					min="2"
					max={MAX_DOTS}
					class="border-black-300 w-full rounded-md border p-2 focus:ring-1 focus:ring-green-500"
				/>
			</div>
			<button on:click={() => generateDots(cant_dots)} class="boton"> Generar Puntos </button>

			<div>
				<p class="mb-2 text-left text-xl font-semibold">Camino: {rutaText}</p>
				<p class="mb-2 text-left text-xl font-semibold">Tiempo: {tiempo}</p>
				<p class="mb-2 text-left text-xl font-semibold">Distancia Total: {distanciaTotal}</p>
			</div>

			<button
				on:click={() => (opSelected = opSelected === 'Aproximado' ? 'Comunidad' : 'Aproximado')}
				class="relative inline-flex h-10 w-52 cursor-pointer items-center rounded-full bg-white"
				role="switch"
				aria-checked={opSelected === 'Aproximado'}
			>
				<span
					class="w-25 absolute top-1 flex h-8 items-center justify-center rounded-full text-white transition-transform duration-200 ease-in-out"
					class:translate-x-full={opSelected === 'Comunidad'}
					class:bg-green-600={opSelected === 'Aproximado'}
					class:bg-[#86B828]={opSelected === 'Comunidad'}
				>
					{opSelected === 'Aproximado' ? 'Aproximado' : 'Comunidad'}
				</span>
				<span
					class="w-22 absolute left-1 top-1 flex h-8 items-center justify-center rounded-full text-gray-700 transition-opacity duration-200 ease-in-out"
					class:opacity-0={opSelected === 'Aproximado'}
					class:opacity-100={opSelected === 'Comunidad'}
				>
					Aproximado
				</span>
				<span
					class="w-22 absolute right-1 top-1 flex h-8 items-center justify-center rounded-full text-gray-700 transition-opacity duration-200 ease-in-out"
					class:opacity-0={opSelected === 'Comunidad'}
					class:opacity-100={opSelected === 'Aproximado'}
				>
					Comunidad
				</span>
			</button>
		</div>
	</div>
</div>

<style>
	.titulo {
		text-align: center;
		font-size: 2em;
		margin-bottom: 20px;
		margin-top: 20px;
		font-weight: bold;
	}

	#mapa {
		height: 550px;
		width: 700px;
		border: 4px solid #1c5a1e;
		border-radius: 0.2rem;
		box-shadow:
			0 25px 50px -12px rgba(0, 0, 0, 0.25),
			0 1.5px 3px 0 rgba(0, 0, 0, 0.07);
	}
	.side {
		flex: 1;
		display: flex;
		flex-direction: column;
		align-items: center;
		justify-content: center;
	}

	.boton {
		width: 100%;
		padding: 0.75rem 2rem;
		background-color: white;
		color: black;
		font-size: 1.125rem;
		border-radius: 0.5rem;
		box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
		transition: background-color 0.2s;
		outline: none;
	}
	.boton:hover {
		background-color: #75a587;
	}
</style>
