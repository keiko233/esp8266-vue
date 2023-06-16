<template>
  <v-list>
    <v-list-item v-for="(item, i) in items" :key="i" :value="item" active-color="indigo" rounded="xl"
      @click="click(item.path)">
      <template v-slot:prepend>
        <v-icon :icon="item.icon" />
      </template>
      <v-list-item-title v-text="item.text" />
    </v-list-item>
  </v-list>
</template>

<script setup lang="ts">
import { useRouter } from 'vue-router';

const router = useRouter();

const items = ref([
  { text: 'Home', icon: 'home', path: '/' },
  { text: 'Fan', icon: '', path: '/fan' },
]);

const click = (path: string) => {
  router.push(path);
};
</script>