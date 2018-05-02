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

   		    var data = {
              fDataSet:[ { fId:"1", fSet: "----" } ],
              fSelectDataId: "0",
              fMinRange: -4,
              fMaxRange: 5,
              fStep: 0.5,
              fRange: [-2,2]
            };
            this.getView().setModel(new JSONModel(data));
   			this._data = data;

   		},

   		OnWebsocketMsg: function(handle, msg){

   			if(msg.indexOf("MODEL:")==0){
   				var json = msg.substr(6);
   				var data = JSROOT.parse(json);

   				if(data) {
   					this.getView().setModel(new JSONModel(data));
   					this._data = data;
   				}
                  console.log("Robust" + this.getView().getModel().getData().fRobust);


   			}
   			else {
               this.getView().byId("SampleText").setText("Get message:\n" + msg);
            }
   		},

   		doSomething: function() {
   			console.log("model=", this.getView().getModel().getProperty("/fSelectDataId"), 
   				                  this.getView().getModel().getData().fSelectTypeId);
       var v1 = this.getView().byId("TypeFunc");
       var v2 = this.getView().byId("Slider");
       console.log("v2 " + v2);
       console.log("v1 value " + v1.getValue());
       console.log("v1 first " + v1.getFirstItem());
       console.log("v1 last " + v1.getLastItem());
       console.log("Slider Range " + v2.getRange());

   			// if (this.websocket)
      //          this.websocket.Send('MODEL:'+this.getView().getModel().getJSON());
      //      	if (this.websocket)
      //      		this.websocket.Send('Range:'+ v1.getValue());
           	

   		},

   		onPanelExit: function(){

   		},
   });
});
