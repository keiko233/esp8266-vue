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
            <v-btn class="ml-2" variant="tonal" @click="getFanSpeed">{{ $t('refresh') }}</v-btn>
          </div>
        </template>
      </v-card>
    </v-col>

    <v-col cols="12" sm="4">
      <v-card :title="$t('control.led_settings')">
        <template v-slot:text>
          <v-overlay v-model="ledLoading" contained class="align-center justify-center">
            <Loading v-if="ledLoading" />
          </v-overlay>
          <div>
            <v-text-field v-model="ledInfo.red.bright" :label="$t('control.red_led_bright_time')" variant="solo" />
            <v-text-field v-model="ledInfo.red.blink" :label="$t('control.red_led_blink_time')" variant="solo" />
            <v-text-field v-model="ledInfo.green.bright" :label="$t('control.green_led_bright_time')" variant="solo" />
            <v-text-field v-model="ledInfo.green.blink" :label="$t('control.green_led_blink_time')" variant="solo" />
            <v-text-field v-model="ledInfo.blue.bright" :label="$t('control.blue_led_bright_time')" variant="solo" />
            <v-text-field v-model="ledInfo.blue.blink" :label="$t('control.blue_led_blink_time')" variant="solo" />
            <v-btn variant="tonal" @click="submitLedInfo">{{ $t('submit') }}</v-btn>
            <v-btn class="ml-2" variant="tonal" @click="getLedInfo">{{ $t('refresh') }}</v-btn>
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

const getFanSpeed = () => {
  fanLoading.value = true;
  fetch('/api/fan')
    .then(response => response.json())
    .then(response => {
      fanLoading.value = false;
      fanSpeed.value = response.speed;
      console.log(response)
    })
    .catch(err => console.error(err));
};

const ledLoading = ref(false);

const ledInfo = ref({
  red: {
    bright: 0,
    blink: 0
  },
  green: {
    bright: 0,
    blink: 0
  },
  blue: {
    bright: 0,
    blink: 0
  }
});

const getLedInfo = () => {
  ledLoading.value = true;
  fetch('/api/led')
    .then(response => response.json())
    .then(response => {
      ledLoading.value = false;
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
  ledLoading.value = true;
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
      ledLoading.value = false;
      console.log(response);
    })
    .catch(err => console.error(err));
};

onMounted(() => {
  getFanSpeed();
  getLedInfo();
});
</script>