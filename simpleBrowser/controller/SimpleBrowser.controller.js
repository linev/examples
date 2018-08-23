sap.ui.define([
   'sap/ui/jsroot/GuiPanelController',
   'sap/ui/model/json/JSONModel'
], function (GuiPanelController, JSONModel) {
   
   "use strict";

   return GuiPanelController.extend("localapp.controller.SimpleBrowser",{

   	// onPanelInit : function() {
    //      // var id = this.getView().getId();
    //      // var opText = this.getView().byId("OperationText");
    //      var data = {
    //            fDataSet:[ { fId:"1", fSet: "----" } ],
    //            fTree: "---"
    //      };
    //      this.getView().setModel(new JSONModel(data));
    //      this._data = data;  
    //   },

   	OnWebsocketMsg: function(handle, msg){

   		if(msg.indexOf("MODEL:") == 0){

   			var json = msg.substr(6);
   			var data = JSROOT.parse(json);

   			if(data){
   				this.getView().setModel(new JSONModel(data));
   				this._data = data;
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