<template>
  <v-row>
    <v-col cols="12" sm="4">
      <v-card :title="$t('settings.static_access_point_settings')">
        <template v-slot:text>
          <v-overlay v-model="loading" contained class="align-center justify-center">
            <Loading v-if="loading" />
          </v-overlay>
          <div>
            <v-text-field v-model="ssid" :label="$t('settings.ssid')" variant="solo" />
            <v-text-field v-model="password" :label="$t('settings.password')" variant="solo" />
            <v-text-field v-model="mdns" :label="$t('settings.mdns')" variant="solo" />
            <v-btn @click="updateWlan">{{ $t('submit') }}</v-btn>
          </div>
        </template>
      </v-card>
    </v-col>

    <v-col cols="12" sm="4">
      <v-card :title="$t('settings.shortcuts')">
        <template v-slot:text>
          <div>
            <v-btn @click="softReboot" block class="mb-2">{{ $t('settings.soft_reboot') }}</v-btn>
            <v-btn @click="restart" block>{{ $t('settings.restart') }}</v-btn>
          </div>
        </template>
      </v-card>
    </v-col>
  </v-row>
</template>

<script setup lang="ts">
const loading = ref(false);

const ssid = ref();
const password = ref();
const mdns = ref();

const updateWlan = () => {
  loading.value = true;
  fetch('/api/settings/wlan', {
    method: 'POST',
    headers: { 'content-type': 'application/x-www-form-urlencoded' },
    body: new URLSearchParams({ 
      ssid: ssid.value,
      password: password.value,
      mdns: mdns.value
    })
  })
    .then(response => response.json())
    .then(response => {
      loading.value = false;
      console.log(response);
    })
    .catch(err => console.error(err));
};

const getWlan = () => {
  loading.value = true;
  fetch('/api/settings/wlan')
    .then(response => response.json())
    .then(response => {
      loading.value = false;
      ssid.value = response.ssid;
      password.value = response.password;
      mdns.value = response.mdns;
    })
    .catch(err => console.error(err));
};

const softReboot = () => {
  fetch('/api/softreboot')
};

const restart = () => {
  fetch('/api/restart')
};

onMounted(() => {
  getWlan();
});
</script>
