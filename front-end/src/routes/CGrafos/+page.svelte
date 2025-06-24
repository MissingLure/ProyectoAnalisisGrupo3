<script lang="ts">
  import { onMount } from 'svelte';
  import { fly } from 'svelte/transition';
  import cgrafo from '../../APIs/Cgrafos';

  const examNames = [
    'Tipo A', 'Tipo B', 'Tipo C', 'Tipo D', 'Tipo E',
    'Tipo F', 'Tipo G', 'Tipo H', 'Tipo I', 'Tipo J'
  ];

  const examColors = [
    '#3b82f6', '#22c55e', '#ef4444', '#f59e0b', '#8b5cf6',
    '#14b8a6', '#e11d48', '#0ea5e9', '#7c3aed', '#a16207'
  ];

  let showModal = false;
  let numEstudiantes = 0;
  const sillasPorMesa = 5;
  const mesasPorFila = 4;
  let numC = 10;
  let algoritmo: 'btColor' | 'btPropio' = 'btColor';

  // Reactividad para el switch
  let algoritmoChecked = false;
  $: algoritmo = algoritmoChecked ? 'btPropio' : 'btColor';

  let aula: number[][][] = [];
  let tiempoEjecucion: number | null = null;

  const generarAula = async () => {
    const algoritmo = algoritmoChecked ? 'btPropio' : 'btColor';
    const values = {
      numEstudiantes,
      numC,
      algoritmo
    };

    try {
      const response = await cgrafo.postCgrafo(values);
      console.log("Respuesta del servidor:", response);

      if (response && Array.isArray(response.result)) {
        const colores: number[] = response.result;
        tiempoEjecucion = response.tiempo;
        const totalMesas = Math.ceil(numEstudiantes / sillasPorMesa);
        const numFilas = Math.ceil(totalMesas / mesasPorFila);

        aula = [];
        let index = 0;

        for (let fila = 0; fila < numFilas; fila++) {
          let filaMesas: number[][] = [];

          for (let mesa = 0; mesa < mesasPorFila; mesa++) {
            if (index >= colores.length) break;

            let mesaSillas: number[] = [];
            for (let silla = 0; silla < sillasPorMesa; silla++) {
              if (index >= colores.length) break;
              mesaSillas.push(colores[index]);
              index++;
            }

            filaMesas.push(mesaSillas);
          }

          aula.push(filaMesas);
        }

      } else {
        console.error('Error: la respuesta del servidor no es válida.');
        alert('No se pudo generar el aula con esa cantidad de colores.');
      }
    } catch (error) {
      console.error('Error al hacer la solicitud:', error);
      alert('Ocurrió un error al contactar al servidor.');
    }

    showModal = false;
  };

  onMount(generarAula);
</script>

<style>
  .classroom {
    background-color: #f5f5f4;
    padding: 2rem;
    min-height: 100vh;
    display: flex;
    flex-direction: column;
    align-items: center;
  }

  .blackboard {
    width: 100%;
    background-color: #1f2937;
    color: white;
    font-size: 1.2rem;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    border-radius: 12px;
    margin-bottom: 2rem;
    padding: 1rem;
  }

  .grid {
    display: flex;
    flex-direction: column;
    gap: 2rem;
  }

  .row {
    display: flex;
    justify-content: center;
    gap: 2rem;
  }

  .table-block {
    display: flex;
    flex-direction: column;
    align-items: center;
  }

  .mesa {
    width: 240px;
    height: 20px;
    background-color: #ffffff;
    border: 2px solid #cbd5e1;
    margin-bottom: 10px;
    border-radius: 6px;
  }

  .sillas {
    display: flex;
    justify-content: space-around;
    width: 100%;
  }

  .silla {
    width: 40px;
    height: 40px;
    border-radius: 50%;
    display: flex;
    align-items: center;
    justify-content: center;
    color: white;
    font-size: 0.6rem;
    font-weight: bold;
    flex-direction: column;
  }

  .silla span {
    font-size: 0.5rem;
  }

  .modal-background {
    position: fixed;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background-color: rgba(0, 0, 0, 0.4);
    display: flex;
    justify-content: center;
    align-items: center;
  }

  .modal {
    background-color: white;
    padding: 2rem;
    border-radius: 12px;
    box-shadow: 0 10px 25px rgba(0, 0, 0, 0.3);
    width: 300px;
    display: flex;
    flex-direction: column;
    gap: 1rem;
  }

  .modal input {
    width: 100%;
    padding: 0.4rem;
    border-radius: 6px;
    border: 1px solid #ccc;
  }

  .modal-actions {
    display: flex;
    flex-direction: column;
    gap: 0.5rem;
  }

  .modal-actions .row-buttons {
    display: flex;
    justify-content: space-between;
    width: 100%;
  }

  .modal-actions button {
    padding: 0.5rem 1rem;
    border-radius: 6px;
    border: none;
    cursor: pointer;
  }

  .modal-actions button:first-child {
    background-color: #22c55e;
    color: white;
  }

  .modal-actions button:last-child {
    background-color: #ef4444;
    color: white;
  }

  .switch {
    position: relative;
    display: inline-block;
    width: 60px;
    height: 30px;
  }

  .switch input {
    opacity: 0;
    width: 0;
    height: 0;
  }

  .slider {
    position: absolute;
    cursor: pointer;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background-color: #ccc;
    transition: 0.4s;
    border-radius: 30px;
  }

  .slider:before {
    position: absolute;
    content: "";
    height: 22px;
    width: 22px;
    left: 4px;
    bottom: 4px;
    background-color: white;
    transition: 0.4s;
    border-radius: 50%;
  }

  input:checked + .slider {
    background-color: #2563eb;
  }

  input:checked + .slider:before {
    transform: translateX(30px);
  }

  .config-button {
    margin-top: 1rem;
    padding: 0.5rem 1rem;
    border-radius: 8px;
    background-color: #2563eb;
    color: white;
    border: none;
    margin-bottom: 1rem;
  }

  .alg-label {
    font-size: 0.85rem;
    margin-top: -0.5rem;
    text-align: center;
  }
</style>

<div class="classroom">
  <div class="blackboard">
    <div>Coloración de Grafos</div>
    {#if tiempoEjecucion !== null}
      <div style="font-size: 0.9rem; margin-top: 4px;">
        Tiempo de ejecución: {tiempoEjecucion} nanosegundos
      </div>
    {/if}
  </div>

  <button class="config-button" on:click={() => showModal = true}>
    Cantidad de Estudiantes
  </button>

  <div class="grid">
    {#each aula as fila, filaIndex (filaIndex)}
      <div class="row" transition:fly="{{ y: 30, delay: filaIndex * 300 }}">
        {#each fila as mesa, mesaIndex (mesaIndex)}
          <div class="table-block" transition:fly="{{ y: 20, delay: mesaIndex * 400 }}">
            <div class="mesa"></div>
            <div class="sillas">
              {#each mesa as color, index (index)}
                <div
                  class="silla"
                  style="background-color: {examColors[color]}"
                  transition:fly="{{ y: 10, duration: 300, delay: index * 600 }}"
                >
                  👨‍🎓
                  <span>{examNames[color]}</span>
                </div>
              {/each}
            </div>
          </div>
        {/each}
      </div>
    {/each}
  </div>

  {#if showModal}
    <div class="modal-background">
      <div class="modal">
        <h2>Configurar Aula</h2>
        <label>
          Total de estudiantes:
          <input type="number" bind:value={numEstudiantes} min="1" />
        </label>
        <div class="modal-actions">
          <div class="row-buttons">
            <button on:click={generarAula}>Aceptar</button>
            <button on:click={() => showModal = false}>Cancelar</button>
          </div>

          <label class="switch">
            <input type="checkbox" bind:checked={algoritmoChecked} />
            <span class="slider"></span>
          </label>
          <div class="alg-label">
            Algoritmo: {algoritmo === 'btPropio' ? 'Propio' : 'Comunidad'}
          </div>
        </div>
      </div>
    </div>
  {/if}
</div>
