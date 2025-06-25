import axios from "./axiosInstance"
/**
 * @param {{ points: { lat: number; lng: number }[],
 *          matriz: number[][],
 *          modo: string,}} values
 */
const postTSP = async (values) => {
    try {
        const response = await axios.post('/api/solve-tsp', values);
        console.log('Received data from backend:', response.data);
        return response.data;
    } catch (error) {
        console.error("Error fetching:", error);
        throw error;
    }
}


export default postTSP;