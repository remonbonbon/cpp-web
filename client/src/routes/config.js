import { dev as isDev } from "$app/environment";

const config = {
  baseURL: isDev ? "http://127.0.0.1:8080" : "",
};
export default config;
