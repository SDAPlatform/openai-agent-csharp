using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text.Encodings.Web;
using System.Text.Json;

namespace hello
{


    public static class Program{

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr _http_post_request_runtime(IntPtr request);
        
        public static void Main(){

        }

        public static string Hello(string s){
            var headers = new Dictionary<string, string>();
            headers.Add("Content-Type" ,"application/json" );
            headers.Add("Authorization" ,"Bearer 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" );
            var ptr = Marshal.StringToHGlobalAnsi(System.Text.Json.JsonSerializer.Serialize(new  {
                URL = "https://api.openai.com/v1/chat/completions",
                Headers = headers,
                Body = "    {\n                \"model\": \"gpt-3.5-turbo\",\n                \"messages\": [{\"role\": \"user\", \"content\": \"_PLACE_HOLDER\"}],\n                \"temperature\": 0.7\n                }".Replace("_PLACE_HOLDER" ,s)
            }));
            var result = _http_post_request_runtime(ptr);
            var result_text = Marshal.PtrToStringAnsi(result);

         using (JsonDocument doc = JsonDocument.Parse(result_text))
        {
            // Access the root element
            JsonElement root = doc.RootElement;

            // Access the message content
            string messageContent = root.GetProperty("choices")[0].GetProperty("message").GetProperty("content").GetString();

           return messageContent;

        }
    }
}
}