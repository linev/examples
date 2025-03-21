sap.ui.define([
   'rootui5/panel/Controller',
   'sap/ui/model/json/JSONModel'
], function (GuiPanelController, JSONModel) {

   "use strict";

   return GuiPanelController.extend("localapp.controller.SimpleBrowser",{

   	onPanelReceive(msg){

   		if(msg.indexOf("MODEL:") == 0){

   			var json = msg.substr(6);
   			var data = JSROOT.parse(json);

   			if(data){
   				var oModel = new JSONModel(data);
               this.getView().setModel(oModel);
               console.log("OMODEL" + oModel);
   				this._data = data;
               oModel.setSizeLimit(1000);
   			}

   			else {
   				//this.getView().byId("SampleText").setText("Get message:\n" + msg);
   			}
   		}
   	},

   	onPanelExit() {

      }

   });

   return;

});