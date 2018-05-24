sap.ui.define([
   'sap/ui/jsroot/GuiPanelController',
   'sap/ui/model/json/JSONModel'
], function (GuiPanelController, JSONModel) {
   "use strict";


   return GuiPanelController.extend("localapp.controller.SimpleFitPanel",{

         //function called from GuiPanelController
      onPanelInit : function() {
         var id = this.getView().getId();
         var opText = this.getView().byId("OperationText");
         var data = {
               fDataSet:[ { fId:"1", fSet: "----" } ],
               fSelectDataId: "0",
               fMinRange: -4,
               fMaxRange: 4,
               fStep: 0.01,
               fRange: [-4,4],
               func: "gaus",
               fOpText: "test"
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
            // console.log("Robust" + this.getView().getModel().getData().fRobust);
            // console.log("Library " + this.getView().getModel().getData().fLibrary);
            // console.log("fSelectXYId " + this.getView().getModel().getData().fSelectXYId);
         }
         else {
            //this.getView().byId("SampleText").setText("Get message:\n" + msg);
         }
      },

      doFit: function() {
         // console.log("model=", this.getView().getModel().getProperty("/fSelectXYId"), 
         //       this.getView().getModel().getProperty("/fSet"));
         //var v1 = this.getView().byId("TypeFunc");

         var data = this.getView().getModel().getData();
         var func = this.getView().byId("TypeXY").getValue();
         console.log("select func " + func);
         data.fRealFunc = func;
         this.getView().getModel().refresh();         

 
         if (this.websocket)
            this.websocket.Send('DOFIT:'+this.getView().getModel().getJSON());
         
         //if (this.websocket)
         //   this.websocket.Send('Range:'+ v1.getValue());
      },

      onPanelExit: function(){

      },

      fOpTextLiveChange: function(oEvent) {
          var sfOpText = oEvent.getParameter("value");
          this.byId("selectedOpText").setText(sfOpText);
      },

      fOpTextChange: function(Controller, JSONModel){
         "use strict";
         var oData = {
            fOpText: "test"
         };
         var fOpTextModel = new JSONModel(oData);
         this.getView().setModel(fOpTextModel);
         this._oData = oData;
         console.log()

      },
      /***STILL WORKING
       onFuncChange: function(){
         var data = this.getView().getModel().getData();
         var linear = this.getView().getModel().getData().fSelectXYId;
         console.log("Linear = ", linear);
         data.fFuncChange = linear;
         this.getView().getModel().refresh();
       },***/

      //change the combo box in Minimization Tab --- Method
      selectRB: function(){
         
         var data = this.getView().getModel().getData();

         var lib = this.getView().getModel().getData().fLibrary;

         console.log('lib = ', lib);
         
         // same code as initialization
         data.fMethodMin = data.fMethodMinAll[parseInt(lib)];
         
         
         // refresh all UI elements
         this.getView().getModel().refresh();
         console.log("Method = ", data.fMethodMinAll[parseInt(lib)]);
         
    },
      //change the combobox in Type Function
      selectTypeFunc: function(){

         var data = this.getView().getModel().getData();

         var typeXY = this.getView().getModel().getData().fSelectTypeId;
         console.log("typeXY = " + typeXY);

         data.fTypeXY = data.fTypeXYAll[parseInt(typeXY)];

         this.getView().getModel().refresh();
         console.log("Type = ", data.fTypeXYAll[parseInt(typeXY)]);
      },


   });

   return 
});