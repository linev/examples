sap.ui.define([
   'sap/ui/jsroot/GuiPanelController',
   'sap/ui/model/json/JSONModel'
], function (GuiPanelController, JSONModel) {
   
   "use strict";

   return GuiPanelController.extend("localapp.controller.SimpleFitPanel",{

         //function called from GuiPanelController
      // onPanelInit : function() {
      //    var id = this.getView().getId();
      //    var opText = this.getView().byId("OperationText");
      //    var data = {
      //          //fDataSet:[ { fId:"1", fSet: "----" } ],
      //          fSelectDataId: "0",
      //          fMinRange: -4,
      //          fMaxRange: 4,
      //          fStep: 0.01,
      //          fRange: [-4,4]
      //    };
      //    this.getView().setModel(new JSONModel(data));
      //    this._data = data;  
      // },

      // Assign the new JSONModel to data      
      OnWebsocketMsg: function(handle, msg){

         if(msg.indexOf("MODEL:")==0){
            var json = msg.substr(6);
            var data = JSROOT.parse(json);

            if(data) {
               this.getView().setModel(new JSONModel(data));
               this._data = data;

            }
         }
         else {
         }
      },

      //Fitting Button
      doFit: function() {
         
         //Data is a new model. With getValue() we select the value of the parameter specified from id
         var data = this.getView().getModel().getData();
         var func = this.getView().byId("TypeXY").getValue();
         //We pass the value from func to C++ fRealFunc
         data.fRealFunc = func;

         var range = this.getView().byId("Slider").getRange();
         console.log("Slider " + range);

         //We pass the values from range array in JS to C++ fRange array
         data.fRange[0] = range[0];
         data.fRange[1] = range[1];

         //Refresh the model
         this.getView().getModel().refresh();         

 
         
         if (this.websocket)
            this.websocket.Send('DOFIT:'+this.getView().getModel().getJSON());
         
         //if (this.websocket)
         //   this.websocket.Send('Range:'+ v1.getValue());
      },

      onPanelExit: function(){

      },
     
     //Change the selected checkboxes depending on Type of Function (TypeXY)
       onTypeXYChange: function(){
         var data = this.getView().getModel().getData();
         var linear = this.getView().getModel().getData().fSelectXYId;
         data.fFuncChange = linear;
         this.getView().getModel().refresh();

         //updates the text area and text in selected tab, depending on the choice in TypeXY ComboBox
         var func = this.getView().byId("TypeXY").getValue();
         this.byId("OperationText").setValueLiveUpdate();
         this.byId("OperationText").setValue(func);
         this.byId("selectedOpText").setText(func);
       },


      //change the combo box in Minimization Tab --- Method depending on Radio Buttons values
      selectRB: function(){
         
         var data = this.getView().getModel().getData();
         var lib = this.getView().getModel().getData().fLibrary;
         
         // same code as initialization
         data.fMethodMin = data.fMethodMinAll[parseInt(lib)];
         
         
         // refresh all UI elements
         this.getView().getModel().refresh();
         console.log("Method = ", data.fMethodMinAll[parseInt(lib)]);
         
    },
      //Change the combobox in Type Function
      //When the Type (TypeFunc) is changed (Predef etc) then the combobox with the funtions (TypeXY), 
      //is also changed 
      selectTypeFunc: function(){

         var data = this.getView().getModel().getData();

         var typeXY = this.getView().getModel().getData().fSelectTypeId;
         console.log("typeXY = " + typeXY);

         data.fTypeXY = data.fTypeXYAll[parseInt(typeXY)];

         this.getView().getModel().refresh();
         console.log("Type = ", data.fTypeXYAll[parseInt(typeXY)]);
      },

      //Change the selected checkbox of Draw Options 
      //if Do not Store is selected then No Drawing is also selected
      storeChange: function(){
         var data = this.getView().getModel().getData();
         var fDraw = this.getView().byId("noStore").getSelected();
         console.log("fDraw = ", fDraw);
         data.fNoStore = fDraw;
         this.getView().getModel().refresh();
         console.log("fNoDrawing ", data.fNoStore);
      },

   });

   return 
});