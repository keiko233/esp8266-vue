<template>
  <v-row>
    <v-col cols="12" sm="4">
      <v-card :title="$t('fan.fan_controll')">
        <template v-slot:text>
          <v-overlay v-model="loading" contained class="align-center justify-center">
            <Loading v-if="loading" />
          </v-overlay>
          <div>
            <p>{{ $t('fan.speed') }}</p>
            <v-slider v-model="speed" class="align-center pt-2 pb-2" max="255" min="0" step="1" hide-details />
            <v-text-field v-model="speed" variant="solo" />
            <v-btn variant="tonal" @click="click">{{ $t('submit') }}</v-btn>
          </div>
        </template>
      </v-card>
    </v-col>
  </v-row>
</template>

<script setup lang="ts">
const loading = ref(false);

const speed = ref(0);

const click = () => {
  loading.value = true;
  fetch('/api/fan', {
    method: 'POST',
    headers: { 'content-type': 'application/x-www-form-urlencoded' },
    body: new URLSearchParams({ speed: String(speed.value) })
  })
    .then(response => response.json())
    .then(response => {
      loading.value = false;
      console.log(response)
    })
    .catch(err => console.error(err));
};
</script>