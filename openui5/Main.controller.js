sap.ui.controller("flexiblecolumnlayout.Main", {

  onInit() {
    console.log('Initizalie main controller')

  },

  onPress(evt) {
    jQuery.sap.require("sap.m.MessageToast");
    sap.m.MessageToast.show(evt.getSource().getId() + " Pressed");
  },

  onPressQuitRoot() {
    globalThis.main_handle?.send('QUIT_ROOT');
  }

});
