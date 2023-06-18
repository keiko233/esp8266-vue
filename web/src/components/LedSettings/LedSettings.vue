<template>
  <v-card :title="$t('control.led_settings')">
    <template v-slot:text>
      <v-overlay v-model="loading" contained class="align-center justify-center">
        <Loading v-if="loading" />
      </v-overlay>
      <div>
        <v-text-field v-model="ledInfo.red.bright" :label="$t('control.red_led_bright_time')" variant="solo" />
        <v-text-field v-model="ledInfo.red.blink" :label="$t('control.red_led_blink_time')" variant="solo" />
        <v-text-field v-model="ledInfo.green.bright" :label="$t('control.green_led_bright_time')" variant="solo" />
        <v-text-field v-model="ledInfo.green.blink" :label="$t('control.green_led_blink_time')" variant="solo" />
        <v-text-field v-model="ledInfo.blue.bright" :label="$t('control.blue_led_bright_time')" variant="solo" />
        <v-text-field v-model="ledInfo.blue.blink" :label="$t('control.blue_led_blink_time')" variant="solo" />
        <v-btn @click="submitLedInfo">{{ $t('submit') }}</v-btn>
        <v-btn class="ml-2" @click="getLedInfo">{{ $t('refresh') }}</v-btn>
      </div>
    </template>
  </v-card>
</template>

<script setup lang="ts">
const loading = ref(false);

const ledInfo = ref({
  red: {
    bright: null,
    blink: null
  },
  green: {
    bright: null,
    blink: null
  },
  blue: {
    bright: null,
    blink: null
  }
});

const getLedInfo = () => {
  loading.value = true;
  fetch('/api/led')
    .then(response => response.json())
    .then(response => {
      loading.value = false;
      ledInfo.value.red.bright = response.ledRedBright;
      ledInfo.value.red.blink = response.ledRedBlink;
      ledInfo.value.green.bright = response.ledGreenBright;
      ledInfo.value.green.blink = response.ledGreenBlink;
      ledInfo.value.blue.bright = response.ledBlueBright;
      ledInfo.value.blue.blink = response.ledBlueBlink;
      console.log(response);
    })
    .catch(err => console.error(err));
};

const submitLedInfo = () => {
  loading.value = true;
  fetch('/api/led', {
    method: 'POST',
    headers: { 'content-type': 'application/x-www-form-urlencoded' },
    body: new URLSearchParams({
      ledRedBright: String(ledInfo.value.red.bright),
      ledRedBlink: String(ledInfo.value.red.blink),
      ledGreenBright: String(ledInfo.value.green.bright),
      ledGreenBlink: String(ledInfo.value.green.blink),
      ledBlueBright: String(ledInfo.value.blue.bright),
      ledBlueBlink: String(ledInfo.value.blue.blink)
    })
  })
    .then(response => response.json())
    .then(response => {
      loading.value = false;
      console.log(response);
    })
    .catch(err => console.error(err));
};

onMounted(() => {
  getLedInfo();
});
</script>