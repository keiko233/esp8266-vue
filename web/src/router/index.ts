import { createRouter, createWebHistory } from 'vue-router'

const routes = [
  {
    path: '/',
    name: 'HomeView',
    component: () => import("../views/HomeView.vue")
  },
  {
    path: '/control',
    name: 'ControlView',
    component: () => import("../views/ControlView.vue")
  },
  {
    path: '/settings',
    name: 'SettingsView',
    component: () => import("../views/SettingsView.vue")
  }
]

const router = createRouter ({
  history: createWebHistory("#"),
  routes: routes,
})

export default router;