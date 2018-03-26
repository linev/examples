sap.ui.define([
   'sap/ui/jsroot/GuiPanelController',
   'sap/ui/model/json/JSONModel'
], function (GuiPanelController, JSONModel) {
   "use strict";

   return GuiPanelController.extend("localapp.controller.TestPanel", {

      // function called from GuiPanelController
      onPanelInit : function() {
         var id = this.getView().getId();
         console.log("Initialization TestPanel id = " + id);
         // such data will be produced on server from TFitPanelModel
         var model = new JSONModel({
            fDataNames:[ { fId:"1", fName: "----" } ],
            fSelectDataId: "0",
            fModelNames: [ { fId:"1", fName: "----" } ],
            fSelectModelId: "0"
         });
         this.getView().setModel(model);
      },

      // function called from GuiPanelController
      onPanelExit : function() {
      },

      OnWebsocketMsg: function(handle, msg, offset) {
         if (typeof msg != "string") {
            console.log('got msg size ' +  msg.byteLength + ' offset ' + offset);
            var arr = new Float32Array(msg, offset);
            console.log('arr len ' + arr.length + ' [0] = ' + arr[0] + ' [77] = ' + arr[77]);
            return;
            
            
            
            var reader = new FileReader;
            reader.onload = function(event) {
               // The file's text will be printed here
                console.log('reader.onload' + (typeof event.target.result));
                var arr = new Float32Array(event.target.result);
                console.log('arr len ' + arr.length + ' [0] = ' + arr[0] + ' [77] = ' + arr[77]);

            }
            reader.readAsArrayBuffer(msg);
            return;
         }

         if (msg.indexOf("MODEL:")==0) {
            var json = msg.substr(6);
            var data = JSROOT.parse(json);

            if (data) {
               this.getView().setModel(new JSONModel(data));
               console.log('TestPanel set new model');
            }

         } else {
            console.log('TestPanel Get message ' + msg);
         }
      },

      handleFitPress : function() {
         console.log('Press fit');
         // To now with very simple logic
         // One can bind some parameters direct to the model and use values from model
         var v1 = this.getView().byId("FitData"),
             v2 = this.getView().byId("FitModel");

         if (this.websocket && v1 && v2)
            this.websocket.Send('DOFIT:"' + v1.getValue() + '","' + v2.getValue() + '"');
      }

   });

});
