export default async function handler(req, res) {
    if (req.method === 'POST') {
      const { image, clip_model_name, mode } = req.body;
  
      // Logic to process `image`, `clip_model_name`, and `mode` parameters
      // Here you can call your CLIP model or another API to generate the prompt
  
      const data = {
        prompt: 'Generated prompt based on image data', // Replace this with the actual generated prompt
        suggestion: 'Ejemplo de sugerencia basada en el prompt',
        destinations: [
          { name: 'Lugar A', coords: [19.432608, -99.133209] },
          { name: 'Lugar B', coords: [40.712776, -74.005974] },
        ],
      };
  
      res.status(200).json(data);  // Return JSON response with prompt and any other necessary data
    } else {
      res.setHeader('Allow', ['POST']);
      res.status(405).end(`Method ${req.method} Not Allowed`);
    }
  }