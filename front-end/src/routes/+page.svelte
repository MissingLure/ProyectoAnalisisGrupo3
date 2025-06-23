<script lang="ts">
  import { goto } from '$app/navigation';
  import { fly } from 'svelte/transition';

  let salir = false;

  function irAlMenu() {
    salir = true;
    // Espera que la animación termine antes de navegar
    setTimeout(() => goto('/Menu'), 600); // 600ms = duración de la animación
  }

  function handleKey(e: KeyboardEvent) {
    if (e.key === "Enter") irAlMenu();
  }
</script>

<!-- Detiene scroll accidental -->
<svelte:window on:keydown={handleKey} />

{#if !salir}
  <div
    transition:fly={{ x: 0, y: 0, duration: 600 }}
    class="flex flex-col items-center justify-center min-h-screen bg-gradient-to-br from-white via-red-400 to-blue-500 text-white"
  >
    <img
    src="/logo.png"
    alt="Logo Unitec"
    class="w-40 h-auto mb-8 object-contain rounded-xl shadow-xl "
    style="background: none;"
  />
    <h1 class="text-5xl font-bold mb-4">Proyecto de Analisis de Algoritmos - Grupo 3</h1>
    <p class="mb-8 text-lg">Set Cover, Coloración de Grafos y Problema de Viajante de Comercio</p>
    <button
      on:click={irAlMenu}
      class="bg-white text-indigo-700 font-bold py-3 px-10 rounded-full shadow-xl hover:bg-indigo-200 transition"
      tabindex="0"
    >
      Presiona Enter o Haz Click para Entrar
    </button>
  </div>
{/if}
