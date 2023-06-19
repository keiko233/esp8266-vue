<template>
  <v-row>
    <v-col cols="12">
      <v-card>
        <template v-slot:text>
          <span>{{ $t('home.current_environment') }}: </span>{{ env }}
        </template>
      </v-card>
    </v-col>
  </v-row>

  <SystemInfo />
</template>

<script setup lang="ts">
const env = ref(import.meta.env.VITE_ENV);
</script>

<style scoped lang="less">
span {
  font-weight: 700;
}
</style>
