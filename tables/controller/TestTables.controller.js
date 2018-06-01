sap.ui.define([
   'sap/ui/jsroot/GuiPanelController',
   'sap/ui/model/json/JSONModel'
], function (GuiPanelController, JSONModel) {
   "use strict";

   return GuiPanelController.extend("localapp.controller.TestTables", {

      // function called from GuiPanelController
      onPanelInit : function() {
         var id = this.getView().getId();
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
         
         
         var t = this.getView().byId("TableM"), pthis = this;
         
         t.getItems().forEach(function(elem, indx) {
            elem.attachBrowserEvent('mouseenter', pthis.itemEnter.bind(pthis, indx, elem));
         });
         
         t = this.getView().byId("TableT");
      },
      
      itemEnter: function(indx, elem, evnt) {
         if (this.websocket)
            this.websocket.Send('LOG:table item enter ' + indx);
      }, 

      // function called from GuiPanelController
      onPanelExit : function() {
      },
      

      OnWebsocketMsg: function(handle, msg, offset) {
         if (typeof msg != "string") {
            // console.log('TestPanel ArrayBuffer size ' +  msg.byteLength + ' offset ' + offset);
            var arr = new Float32Array(msg, offset);
            return;
        }

          if (msg.indexOf("MODEL:")==0) {
            var json = msg.substr(6);
            var data = JSROOT.parse(json);

            if (data) {
               this.getView().setModel(new JSONModel(data));
            }

        } else {
            // this.getView().byId("SampleText").setText("Get message:\n" + msg);
         }
      },
      
      handleGetBinary: function() {
         // just request binary data
         if (this.websocket)
            this.websocket.Send("GET_BINARY");
      },

      handleFitPress : function() {
         // To now with very simple logic
         // One can bind some parameters direct to the model and use values from model
         var v1 = this.getView().byId("FitData"),
             v2 = this.getView().byId("FitModel");

         if (this.websocket && v1 && v2)
            this.websocket.Send('DOFIT:"' + v1.getValue() + '","' + v2.getValue() + '"');
         console.log("test!" + v1);
      }

   });

});
