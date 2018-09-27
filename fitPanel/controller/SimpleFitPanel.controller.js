sap.ui.define([
   'sap/ui/jsroot/GuiPanelController',
   'sap/ui/model/json/JSONModel'
], function (GuiPanelController, JSONModel) {
   var dataId; 
   var typeId;
   var funcId;
   var operationId;
   var methodId;
   var linearId;
   var robustId;
   var libraryId;
   var printId;
   var methodMinId;
   "use strict";

   return GuiPanelController.extend("localapp.controller.SimpleFitPanel",{

         //function called from GuiPanelController
      onPanelInit : function() {
         var id = this.getView().getId();
         var opText = this.getView().byId("OperationText");
         var data = {
               //fDataSet:[ { fId:"1", fSet: "----" } ],
               fSelectDataId: "0",
               fMinRange: -4,
               fMaxRange: 4,
               fStep: 0.01,
               fRange: [-4,4]
         };
         this.getView().setModel(new JSONModel(data));
         this._data = data;  
      },

      // Assign the new JSONModel to data      
      OnWebsocketMsg: function(handle, msg){

         if(msg.startsWith("MODEL:")){
            var json = msg.substr(6);
            var data = JSROOT.parse(json);
            dataSetId = data.fSelectDataId;
            typeFuncId = data.fSelectTypeId;
            typeXYId = data.fSelectXYId;
            operationId = data.fOperation;
            methodId = data.fSelectMethodId;
            linearId = data.fLinear;
            robustId = data.fRobust;
            libraryId = data.fLibrary;
            printId = data.fPrint;
            methodMinId = data.fSelectMethodMinId;

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
      },

      onPanelExit: function(){

      },

      resetPanel: function(oEvent){

         var comboDataSet = this.byId("DataSet").setSelectedKey(dataSetId);
         var comboTypeFunc = this.byId("TypeFunc").setSelectedKey(typeFuncId);
         var comboTypeXY = this.byId("TypeXY").setSelectedKey(typeXYId);
         var comboMethod = this.byId("MethodCombo").setSelectedKey(methodId);
         var comboMethodMin = this.byId("MethodMin").setSelectedKey(methodMinId);

         

         var radioOperation = this.byId("RBOperation").setSelectedIndex(operationId);
         var radioLibraryRB = this.byId("LibraryRB").setSelectedIndex(libraryId);
         var radioPrint = this.byId("Print").setSelectedIndex(printId);

         var textAreaOperationText = this.byId("OperationText").setValue();
         var textSelectedOpText = this.byId("selectedOpText").setText();
         var inputTestError = this.byId("testError").setValue();
         var inputMaxTolerance = this.byId("maxTolerance").setValue();
         var inputMaxInterations = this.byId("maxInterations").setValue(); 
   

         var checkLinearFit = this.byId("linearFit").setSelected(linearId);
         var checkRobust = this.byId("robust").setSelected(robustId);

         
         var otab = this.byId("Fit_Options");
         var bcheck = oEvent.getParameter("selected");
         otab.getItems().forEach(function(item){
            var fitCol1 = item.getCells()[0];
            var fitCol2 = item.getCells()[1];

            fitCol1.setSelected(bcheck);
            fitCol2.setSelected(bcheck);
         });
         
         var ftab = this.byId("Draw");
         var fcheck = oEvent.getParameter("selected");
         ftab.getItems().forEach(function(item){
            var col1 = item.getCells()[0];

            col1.setSelected(fcheck);
            
         });

         var sRange = [-4,4];
         var stepInputRobustStep = this.byId("RobustStep").setValue(0.95);
         var rangeSlider = this.byId("Slider").setRange(sRange);




      },
     
     //Change the input text field. When a function is seleced, it appears on the text input field and
     //on the text area.
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