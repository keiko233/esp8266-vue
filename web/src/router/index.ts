import { createRouter, createWebHistory } from 'vue-router'

const routes = [
  {
    path: '/',
    name: 'HomeView',
    component: () => import("../views/HomeView.vue")
  },
  {
    path: '/fan',
    name: 'FanView',
    component: () => import("../views/FanView.vue")
  }
]

const router = createRouter ({
  history: createWebHistory("/"),
  routes: routes,
})

export default router;