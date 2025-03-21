sap.ui.define([
   'rootui5/panel/Controller',
   'sap/ui/model/json/JSONModel'
], function (GuiPanelController, JSONModel) {

   "use strict";

   return GuiPanelController.extend("localapp.controller.SimpleBrowser",{

   	onPanelReceive(msg){

   		if(msg.indexOf("MODEL:") == 0){

   			const json = msg.slice(6),
   			      data = JSON.parse(json);

   			if(data){
   				const oModel = new JSONModel(data);
               this.getView().setModel(oModel);
               console.log("OMODEL" + oModel);
   				this._data = data;
               oModel.setSizeLimit(1000);
   			}

   			else {
   			}
   		}
   	},

   	onPanelExit() {
      }

   });

   return;

});