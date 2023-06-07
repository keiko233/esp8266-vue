import { defineConfig, loadEnv } from 'vite'
import vue from '@vitejs/plugin-vue'
import AutoImport from 'unplugin-auto-import/vite'
import Components from 'unplugin-vue-components/vite'
import vuetify from 'vite-plugin-vuetify'

// https://vitejs.dev/config/
export default ({ mode }) => defineConfig({
  plugins: [
    vue(),
    AutoImport({
      imports: [
        'vue'
      ]
    }),
    Components({
      resolvers: []
    }),
    vuetify({ 
      autoImport: true 
    })
  ],
  build: {
    outDir: '../data/web',
    assetsDir: "./",
    rollupOptions: {
      output: {
        entryFileNames: `[name].js`,
        chunkFileNames: `[name].js`,
        assetFileNames: `[name].[ext]`,
        format: 'umd'
      }
    }
  },
  server: {
    proxy: {
      '/api': {
        target: loadEnv(mode, './').VITE_API_URL,
        changeOrigin: true,
        rewrite: (path) => path.replace(/^\/api/, '')
      }
    }
 },
})
