import { createApp } from 'vue'
import './assets/index.less'
import App from './App.vue'
import i18n from './i18n'
import router from './router'

// Vuetify
import 'vuetify/styles'
import { createVuetify } from 'vuetify'
import { aliases, md } from 'vuetify/iconsets/md'

const vuetify = createVuetify({
  icons: {
    defaultSet: 'md',
    aliases,
    sets: {
      md,
    },
  },
})

createApp(App)
  .use(vuetify)
  .use(i18n)
  .use(router)
  .mount('#app')

