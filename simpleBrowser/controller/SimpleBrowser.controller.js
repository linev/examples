sap.ui.define([
   'sap/ui/jsroot/GuiPanelController',
   'sap/ui/model/json/JSONModel'
], function (GuiPanelController, JSONModel) {
   
   "use strict";

   return GuiPanelController.extend("localapp.controller.SimpleBrowser",{



   	OnWebsocketMsg: function(handle, msg){

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

   	onPanelExit: function(){

      },

   });

   return;

});