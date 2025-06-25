import axios from "../APIs/axiosInstance"
/**
 * @param {{ numEstudiantes: number,
 *          numC: number  }} values
 */
const postCgrafo = async (values) => {
    try {
        const response = await axios.post('/api/cgrafo',values);
        return response.data;
    } catch (error) {
        console.error("Error fetching colores:", error);
        throw error;
    }
}
/**
 * @param {{ numEstudiantes: number,
 *          numC: number  }} values
 */
const postCgrafoPropio = async (values) => {
    try {
        const response = await axios.post('/api/cgrafo',values);
        return response.data;
    } catch (error) {
        console.error("Error fetching colores:", error);
        throw error;
    }
}

export default {postCgrafo,postCgrafoPropio};