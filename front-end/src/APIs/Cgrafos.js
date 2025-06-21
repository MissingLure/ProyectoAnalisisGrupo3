import axios from "../APIs/axiosInstance"
/**
 * @param {{ numEstudiantes: number,
 *          numC: number  }} values
 */
const postCgrafo = async (values) => {
    try {
        const response = await axios.post('/CGrafos',values);
        return response.data;
    } catch (error) {
        console.error("Error fetching colores:", error);
        throw error;
    }
}

export default postCgrafo;