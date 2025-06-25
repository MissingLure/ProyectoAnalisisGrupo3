<script lang="ts">
  import { fly } from 'svelte/transition';

  // Podés ponerle animación de entrada
  let entrar = true;

  // Función para cada botón (por ahora solo logs, después le metés la navegación real)
  import { goto } from '$app/navigation';

function goTo(opcion: string) {
  switch (opcion) {
    case 'Set Cover':
      goto('/SetCover');
      break;
    case 'Problema del Viajante de Comercio':
      goto('/TSP');
      break;
    case 'Coloración de Grafos':
      goto('/CGrafos');
      break;
    default:
      goto('/Menu'); // O poné una ruta de error si querés
  }
}
</script>

<div
  transition:fly="{{ y: 60, duration: 600 }}"
  class="flex flex-col items-center justify-center min-h-screen bg-gradient-to-br from-indigo-700 via-purple-600 to-pink-500 text-white"
>
  <span class="text-7xl mb-6">🧩</span>
  <h1 class="text-4xl font-bold mb-4">Menú Principal</h1>
  <p class="mb-8 text-lg">Selecciona un problema a visualizar:</p>
  <div class="flex flex-col gap-6 w-full max-w-xs">
    <button
      class="bg-white text-indigo-700 font-semibold py-3 px-8 rounded-full shadow-lg hover:bg-indigo-200 transition text-lg"
      on:click={() => goTo('Set Cover')}
    >
      Set Cover
    </button>
    <button
      class="bg-white text-indigo-700 font-semibold py-3 px-8 rounded-full shadow-lg hover:bg-indigo-200 transition text-lg"
      on:click={() => goTo('Problema del Viajante de Comercio')}
    >
      Problema del Viajante de Comercio
    </button>
    <button
      class="bg-white text-indigo-700 font-semibold py-3 px-8 rounded-full shadow-lg hover:bg-indigo-200 transition text-lg"
      on:click={() => goTo('Coloración de Grafos')}
    >
      Coloración de Grafos
    </button>
  </div>
  <footer class="absolute bottom-6 text-white text-xs opacity-70">
    Grupo 3 - Analisis de Algoritmos Q2 2025
  </footer>
</div>
