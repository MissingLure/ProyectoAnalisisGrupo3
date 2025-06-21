<script lang="ts">
  import { onMount } from 'svelte';
  import postCgrafo from '../../APIs/Cgrafos';
  const examNames = ['Matemáticas', 'Física', 'Biología'];
  const examColors = ['#3b82f6', '#22c55e', '#ef4444'];

  let showModal = false;
  let numEstudiantes = 0;
  let sillasPorMesa = 5;
  let mesasPorFila = 4;
  let numC = 5;
  let aula: number[][][] = [];

  function generarAula() {
    const values = {
      numEstudiantes,
      numC
    };
    const fetchColores = async () => {
      try {
        const response = await postCgrafo(values);
        if (response && response.colores) {
          console.log('Colores obtenidos del servidor:', response.colores);
          return response.colores;
        } else {
          console.error('Error al obtener los colores del servidor');
          return examColors; // Fallback to default colors
        }
      } catch (error) {
        console.error('Error al hacer la solicitud:', error);
        return examColors; // Fallback to default colors
      }
    };
    const totalMesas = Math.ceil(numEstudiantes / sillasPorMesa);
    const numFilas = Math.ceil(totalMesas / mesasPorFila);
    
    aula = [];
    let estudiantesAsignados = 0;
     
    for (let fila = 0; fila < numFilas; fila++) {
      let filaMesas: number[][] = [];

      for (let mesa = 0; mesa < mesasPorFila; mesa++) {
        if (estudiantesAsignados >= numEstudiantes) break;

        let mesaSillas: number[] = [];
        for (let silla = 0; silla < sillasPorMesa; silla++) {
          if (estudiantesAsignados >= numEstudiantes) break;
          mesaSillas.push(Math.floor(Math.random() * examNames.length));
          estudiantesAsignados++;
        }

        filaMesas.push(mesaSillas);
      }

      aula.push(filaMesas);
    }

    showModal = false;
  }

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
    height: 80px;
    background-color: #1f2937;
    color: white;
    font-size: 1.2rem;
    display: flex;
    justify-content: center;
    align-items: center;
    border-radius: 12px;
    margin-bottom: 2rem;
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
    background-color: #dc2626;
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

  footer {
    margin-top: 2rem;
    font-size: 0.8rem;
    color: #6b7280;
  }

  /* Modal styles */
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
    justify-content: space-between;
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

  .config-button {
    margin-top: 1rem;
    padding: 0.5rem 1rem;
    border-radius: 8px;
    background-color: #2563eb;
    color: white;
    border: none;
  }
</style>

<div class="classroom">
  <div class="blackboard">🧑‍🏫 Aula de Exámenes - Estilo Realista</div>

  <button class="config-button" on:click={() => showModal = true}>
    Configurar Aula
  </button>

  <div class="grid">
    {#each aula as fila}
      <div class="row">
        {#each fila as mesa}
          <div class="table-block">
            <div class="mesa"></div>
            <div class="sillas">
              {#each mesa as color}
                <div class="silla" style="background-color: {examColors[color]}">
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
        <label>
          Sillas por mesa:
          <input type="number" bind:value={sillasPorMesa} min="1" />
        </label>
        <label>
          Mesas por fila:
          <input type="number" bind:value={mesasPorFila} min="1" />
        </label>

        <div class="modal-actions">
          <button on:click={generarAula}>Aceptar</button>
          <button on:click={() => showModal = false}>Cancelar</button>
        </div>
      </div>
    </div>
  {/if}

  <footer>Grupo 3 - Análisis de Algoritmos Q2 2025</footer>
</div>
