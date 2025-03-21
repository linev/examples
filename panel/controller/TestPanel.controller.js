sap.ui.define([
   'rootui5/panel/Controller',
   'sap/ui/model/json/JSONModel'
], function (GuiPanelController, JSONModel) {
   "use strict";

   return GuiPanelController.extend("localapp.controller.TestPanel", {

      // function called from panel.Controller
      onPanelInit() {
         // dummy model for initialization
         let model = new JSONModel({
            fDataNames:[ { fId:"1", fName: "----" } ],
            fSelectDataId: "0",
            fModelNames: [ { fId:"1", fName: "----" } ],
            fSelectModelId: "0"
         });
         this.getView().setModel(model);

         if (document) document.title = "TestPanel";
      },

      // function called from GuiPanelController
      onPanelExit() {
      },


      onPanelReceive(msg, offset) {
         if (typeof msg != "string") {
            // console.log('TestPanel ArrayBuffer size ' +  msg.byteLength + ' offset ' + offset);
            let arr = new Float32Array(msg, offset);

            this.getView().byId("SampleText").setText("Got binary as float array\n" +
                  'array length ' + arr.length + '\n' +
                  ' [0] = ' + arr[0] + '\n' +
                  ' [7] = ' + arr[7] + '\n' +
                  ' [last] = ' + arr[arr.length-1]);

            return;
         }

         if (msg.indexOf("MODEL:")==0) {
            var json = msg.slice(6);
            var data = JSON.parse(json);

            this.getView().byId("SampleText").setText("Model size:" + json.length);

            if (data)
               this.getView().setModel(new JSONModel(data));

         } else {
            this.getView().byId("SampleText").setText("Get message:\n" + msg);
         }
      },

      handleGetBinary() {
         // just request binary data
         this.panelSend("GET_BINARY");
      },

      handleFitPress() {
         // To now with very simple logic
         // One can bind some parameters direct to the model and use values from model
         var v1 = this.getView().byId("FitData"),
             v2 = this.getView().byId("FitModel");

         if (v1 && v2)
            this.panelSend('DOFIT:"' + v1.getValue() + '","' + v2.getValue() + '"');
         console.log("test!" + v1);
      }

   });

});
