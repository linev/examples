sap.ui.define([
   'sap/ui/jsroot/GuiPanelController',
   'sap/ui/model/json/JSONModel'
], function (GuiPanelController, JSONModel) {
   "use strict";


   return GuiPanelController.extend("localapp.controller.SimpleFitPanel",{

   		//function called from GuiPanelController
   		onPanelInit : function() {
   			console.log("I am A");
   			var id = this.getView().getId();
   			var model = new JSONModel({
              fDataSet:[ { fId:"1", fSet: "----" } ],
              fSelectDataId: "0",
              fMinRange: -4,
              fMaxRange: 5,
              fStep: 0.5,
              fRange: [-2,2]
            });
            this.getView().setModel(model);
   			console.log("I am B");
   		},

   		OnWebsocketMsg: function(handle, msg){

   			if(msg.indexOf("MODEL:")==0){
   				var json = msg.substr(6);
   				var data = JSROOT.parse(json);

   				if(data) {
   					this.getView().setModel(new JSONModel(data));
   				}

   			}
   			else {
            this.getView().byId("SampleText").setText("Get message:\n" + msg);
         }
   		},

   		onPanelExit: function(){

   		}


   });
   	

});
