sap.ui.define([
   'rootui5/panel/Controller',
   'sap/ui/model/json/JSONModel'
], function (GuiPanelController, JSONModel) {
   "use strict";

   return GuiPanelController.extend("localapp.controller.TestTables", {

      // function called from GuiPanelController
      onPanelInit() {
         let id = this.getView().getId();
         console.log("Initialization TestPanel id = " + id);


         var oData = [{
               width: "auto",
               header: "Product Name",
               demandPopin: false,
               minScreenWidth: "",
               styleClass: "cellBorderLeft cellBorderRight"
            }, {
               width: "20%",
               header: "Supplier Name",
               demandPopin: false,
               minScreenWidth: "",
               styleClass: "cellBorderRight"
            }, {
               width: "50%",
               header: "Description",
               demandPopin: true,
               minScreenWidth: "Tablet",
               styleClass: "cellBorderRight"
            }
         ];

         var oData2 = [ {
               Name: "abc1",
               SupplierName: "abc1 title",
               Description: "abc1 description"
            }, {
               Name: "abc1",
               SupplierName: "abc1 title",
               Description: "abc1 description",
               highlight: "Information"
            }, {
               Name: "abc2",
               SupplierName: "abc2 title",
               Description: "abc2 description"
            }
         ];

         this.oColumnModel = new JSONModel();
         this.oColumnModel.setData(oData);
         this.getView().setModel(this.oColumnModel, "columns");

         this.oProductsModel = new JSONModel();
         this.oProductsModel.setData(oData2);
         this.getView().setModel(this.oProductsModel, "products");

         var t = this.getView().byId("TableT"), pthis = this;
         t.onAfterRendering0 = t.onAfterRendering;
         t.onAfterRendering = function() {
            this.onAfterRendering0();
            pthis.onAfterTRendering();
         }

      },

      onAfterTRendering() {

         var t = this.getView().byId("TableT"), pthis = this;
         t.getRows().forEach(function(elem, indx) {
            elem.$().mouseenter(pthis.itemEnter.bind(pthis, indx, elem));
         });

      },

      onAfterRendering() {
         console.log("ON AFTER RENDERING");

         var t = this.getView().byId("TableM"), pthis = this;
         t.getItems().forEach(function(elem, indx) {
            elem.$().mouseenter(pthis.itemEnter.bind(pthis, indx, elem));
         });

      },

      itemEnter(indx, elem, evnt) {
         if (this.websocket)
            this.websocket.Send('LOG:table item enter ' + indx);
//         if (elem && elem.$)
//            elem.$().css('background-color', 'red')
//                    .css('textcolor', 'green')
//                    .css('font-size', '25px');
      },

      // function called from GuiPanelController
      onPanelExit() {
      },


      onPanelReceive(msg, offset) {
         if (typeof msg != "string") {
            // console.log('TestPanel ArrayBuffer size ' +  msg.byteLength + ' offset ' + offset);
            let arr = new Float32Array(msg, offset);
            return;
        }

         if (msg.indexOf("MODEL:")==0) {
            const json = msg.slice(6),
                  data = this.jsroot.parse(json);

            if (data) {
               this.getView().setModel(new JSONModel(data));
            }

        } else {
            // this.getView().byId("SampleText").setText("Get message:\n" + msg);
         }
      },

      handleGetBinary() {
         // just request binary data
         this.panelSend('GET_BINARY');
      },

      handleFitPress() {
         // To now with very simple logic
         // One can bind some parameters direct to the model and use values from model
         let v1 = this.getView().byId("FitData"),
             v2 = this.getView().byId("FitModel");

         if (v1 && v2)
            this.panelSend('DOFIT:"' + v1.getValue() + '","' + v2.getValue() + '"');
      }

   });

});
