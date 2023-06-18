<template>
  <v-card :title="$t('control.fan_controll')">
    <template v-slot:text>
      <v-overlay v-model="loading" contained class="align-center justify-center">
        <Loading v-if="loading" />
      </v-overlay>
      <div>
        <p>{{ $t('control.speed') }}</p>
        <v-slider v-model="fanSpeed" class="align-center pt-2 pb-2" max="255" min="0" step="1" hide-details />
        <v-text-field v-model="fanSpeed" variant="solo" />
        <v-btn @click="submitFanSpeed">{{ $t('submit') }}</v-btn>
        <v-btn class="ml-2" @click="getFanSpeed">{{ $t('refresh') }}</v-btn>
      </div>
    </template>
  </v-card>
</template>

<script setup lang="ts">
const loading = ref(false);

const fanSpeed = ref(0);

const submitFanSpeed = () => {
  loading.value = true;
  fetch('/api/fan', {
    method: 'POST',
    headers: { 'content-type': 'application/x-www-form-urlencoded' },
    body: new URLSearchParams({ speed: String(fanSpeed.value) })
  })
    .then(response => response.json())
    .then(response => {
      loading.value = false;
      console.log(response)
    })
    .catch(err => console.error(err));
};

const getFanSpeed = () => {
  loading.value = true;
  fetch('/api/fan')
    .then(response => response.json())
    .then(response => {
      loading.value = false;
      fanSpeed.value = response.speed;
      console.log(response)
    })
    .catch(err => console.error(err));
};

onMounted(() => {
  getFanSpeed();
});
</script>