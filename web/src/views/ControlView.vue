<template>
  <v-row>
    <v-col cols="12" sm="4">
      <v-card :title="$t('control.fan_controll')">
        <template v-slot:text>
          <v-overlay v-model="fanLoading" contained class="align-center justify-center">
            <Loading v-if="fanLoading" />
          </v-overlay>
          <div>
            <p>{{ $t('control.speed') }}</p>
            <v-slider v-model="fanSpeed" class="align-center pt-2 pb-2" max="255" min="0" step="1" hide-details />
            <v-text-field v-model="fanSpeed" variant="solo" />
            <v-btn variant="tonal" @click="submitFanSpeed">{{ $t('submit') }}</v-btn>
          </div>
        </template>
      </v-card>
    </v-col>
  </v-row>
</template>

<script setup lang="ts">
const fanLoading = ref(false);

const fanSpeed = ref(0);

const submitFanSpeed = () => {
  fanLoading.value = true;
  fetch('/api/fan', {
    method: 'POST',
    headers: { 'content-type': 'application/x-www-form-urlencoded' },
    body: new URLSearchParams({ speed: String(fanSpeed.value) })
  })
    .then(response => response.json())
    .then(response => {
      fanLoading.value = false;
      console.log(response)
    })
    .catch(err => console.error(err));
};
</script>