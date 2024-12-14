// Plugins
import vue from '@vitejs/plugin-vue'
import vuetify, { transformAssetUrls } from 'vite-plugin-vuetify'

// Utilities
import { defineConfig } from 'vite'
import { fileURLToPath, URL } from 'node:url'
import viteCompression from 'vite-plugin-compression';

// https://vitejs.dev/config/
export default defineConfig({
  build: {
    outDir: './dist',
    assetsDir: "./",
    assetsInlineLimit: 4096 * 4,
    cssCodeSplit: false,
    rollupOptions: {
      output: {
        dir: './dist',
        entryFileNames: 'plugin.js',
        assetFileNames: 'plugin.css',
        chunkFileNames: "chunk.js",
        manualChunks: undefined,
      }
    }
  },
  plugins: [
    viteCompression(),
    vue({
      template: { transformAssetUrls },
    }),
    // https://github.com/vuetifyjs/vuetify-loader/tree/master/packages/vite-plugin#readme
    vuetify({
      autoImport: true,
    }),
  ],
  define: {
    'process.env': {
      "NODE_ENV": "production"
    }
  },
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url)),
    },
    extensions: ['.js', '.json', '.jsx', '.mjs', '.ts', '.tsx', '.vue'],
  },
  server: {
    port: 3000,
  },
})


