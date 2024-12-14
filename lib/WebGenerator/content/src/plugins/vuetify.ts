/**
 * plugins/vuetify.ts
 *
 * Framework documentation: https://vuetifyjs.com`
 */

// Styles
import "vuetify/styles";

// Composables
import { createVuetify } from "vuetify";
import { aliases, mdi } from "vuetify/iconsets/mdi-svg";
import {
  mdiAccount,
  mdiBasket,
  mdiPencil,
  mdiPlus,
  mdiTrashCan,
} from "@mdi/js";

// https://vuetifyjs.com/en/introduction/why-vuetify/#feature-guides
export default createVuetify({
  theme: {
    themes: {
      light: {
        colors: {
          primary: "#1867C0",
          secondary: "#5CBBF6",
        },
      },
    },
  },
  icons: {
    defaultSet: "mdi",
    aliases: {
      ...aliases,
      account: mdiAccount,
      pencil: mdiPencil,
      delete: mdiTrashCan,
      add: mdiPlus,
    },
    sets: {
      mdi,
    },
  },
});
