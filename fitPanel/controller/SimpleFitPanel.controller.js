sap.ui.define([
   'rootui5/panel/Controller',
   'sap/ui/model/json/JSONModel',
   'sap/ui/unified/ColorPickerPopover',
   'sap/m/Button',
   'sap/m/Table'
], function (GuiPanelController, JSONModel, ColorPickerPopover, Button, Table) {

   "use strict";
   let count = 0;
   return GuiPanelController.extend("localapp.controller.SimpleFitPanel",{

         //function called from GuiPanelController
      onPanelInit() {
         console.log('INIT PANEL');

         let id = this.getView().getId();
         this.inputId = "";
         let opText = this.getView().byId("OperationText");
         let data = {
               //fDataSet:[ { fId:"1", fSet: "----" } ],
               fSelectDataId: "2",
               // fMinRange: -4,
               // fMaxRange: 4,
               fStep: 0.01,
               fRange: [-4,4],
               fUpdateRange: [-4,4]
         };
         this.getView().setModel(new JSONModel(data));
         this._data = data;
      },



      // Assign the new JSONModel to data
      onPanelReceive(msg){

         if(msg.startsWith("MODEL:")){
            let json = msg.substr(6);
            let data = this.jsroot.parse(json);

            if(data) {
               this.getView().setModel(new JSONModel(data));
               this._data = data;

               this.copyModel = Object.assign({},data);
            }
         }

      },

      //Fitting Button
      doFit() {
         //Keep the #times the button is clicked
         count++;
         //Data is a new model. With getValue() we select the value of the parameter specified from id
         let data = this.getView().getModel().getData();
         //let func = this.getView().byId("TypeXY").getValue();
         let func = this.getView().byId("selectedOpText").getText();
         //We pass the value from func to C++ fRealFunc
         data.fRealFunc = func;

         //Refresh the model
         this.getView().getModel().refresh();
         //Each time we click the button, we keep the current state of the model
         this.copyModel[count] = Object.assign({},data);

         this.panelSend('DOFIT:'+this.getView().getModel().getJSON());
      },

      onPanelExit(){

      },

      resetPanel(oEvent){

         if(!this.copyModel) return;

         Object.assign(this._data, this.copyModel);
         this.getView().getModel().updateBindings();
         this.byId("selectedOpText").setText("gaus");
         this.byId("OperationText").setValue("");
      },

      backPanel() {
         //Each time we click the button, we go one step back
         count--;
         if(count < 0) return;
         if(!this.copyModel[count]) return;

         Object.assign(this._data, this.copyModel[count]);
         this.getView().getModel().updateBindings();
      },

      backPanel() {
         //Each time we click the button, we go one step back
         count--;
         if(count < 0) return;
         if(!this.copyModel[count]) return;

         Object.assign(this._data, this.copyModel[count]);
         this.getView().getModel().updateBindings();
         return;
      },

     //Change the input text field. When a function is selected, it appears on the text input field and
     //on the text area.
      onTypeXYChange(){
         let data = this.getView().getModel().getData();
         let linear = this.getView().getModel().getData().fSelectXYId;
         data.fFuncChange = linear;
         this.getView().getModel().refresh();

         //updates the text area and text in selected tab, depending on the choice in TypeXY ComboBox
         let func = this.getView().byId("TypeXY").getValue();
         this.byId("OperationText").setValueLiveUpdate();
         this.byId("OperationText").setValue(func);
         this.byId("selectedOpText").setText(func);
      },

      operationTextChange(oEvent) {
         let newValue = oEvent.getParameter("value");
         this.byId("selectedOpText").setText(newValue);
      },


      //change the combo box in Minimization Tab --- Method depending on Radio Buttons values
      selectRB(){

         let data = this.getView().getModel().getData();
         let lib = this.getView().getModel().getData().fLibrary;

         // same code as initialization
         data.fMethodMin = data.fMethodMinAll[parseInt(lib)];


         // refresh all UI elements
         this.getView().getModel().refresh();
         console.log("Method = ", data.fMethodMinAll[parseInt(lib)]);

    },
      //Change the combobox in Type Function
      //When the Type (TypeFunc) is changed (Predef etc) then the combobox with the funtions (TypeXY),
      //is also changed
      selectTypeFunc(){

         let data = this.getView().getModel().getData();

         let typeXY = this.getView().getModel().getData().fSelectTypeId;
         let dataSet = this.getView().getModel().getData().fSelectDataId;
         console.log("typeXY = " + dataSet);

         data.fTypeXY = data.fTypeXYAll[parseInt(typeXY)];

         this.getView().getModel().refresh();
         console.log("Type = ", data.fTypeXYAll[parseInt(typeXY)]);
      },

      //Change the selected checkbox of Draw Options
      //if Do not Store is selected then No Drawing is also selected
      storeChange(){
         let data = this.getView().getModel().getData();
         let fDraw = this.getView().byId("noStore").getSelected();
         console.log("fDraw = ", fDraw);
         data.fNoStore = fDraw;
         this.getView().getModel().refresh();
         console.log("fNoDrawing ", data.fNoStore);
      },

      setParametersDialog(){
         let aColumnData = [{
            columnId: "Name"
         },
         {
            columnId: "Fix"
         },
         {
            columnId: "Bound"
         },
         {
            columnId: "Value"
         },
         {
            columnId: "Min"
         },
         {
            columnId: "Range"
         },
         {
            columnId: "Max"
         },
         {
            columnId: "Steps"
         },
         {
            columnId: "Errors"
         }];

         let colModel = new sap.ui.model.json.JSONModel();
         colModel.setData({
            columns: aColumnData
         });

         let oPersonalizationDialog = sap.ui.xmlfragment("localapp.view.SetParameters", this);
         this.getView().addDependent(oPersonalizationDialog);
         oPersonalizationDialog.open();
         let func = this.getView().byId("selectedOpText").getText();
         let oTable = new sap.m.Table({});
         oTable.setModel(colModel);

         oTable.bindAggregation("columns", "/columns", function(index, context) {
            return new sap.m.Column({
               header: new sap.m.Label({
                  text: context.getObject().columnId
               }),
            });
         });

         oTable.bindItems("/rows", function(index, context) {
            let obj = context.getObject();
            let row = new sap.m.ColumnListItem();

            for(let k in obj) {
               row.addCell(new sap.m.Text({
                  text: obj[k]
               }));
            }

            return row;
         });

         oTable.placeAt("content");

         // if (func == "gaus"){
         //    let par = 3;
         //    for (let i=0; i<par; i++){
         //       for (let j=0; j<8; j++){

         //       }
         //       //oPersonalizationDialog.addContent(new sap.m.Title)
         //    }
         // }
         // for (let i=0; i<5; i++){
         //    oTable.addContent(new sap.m.Label({
         //       text: "label",
         //       columns: [ new sap.m.Column({
         //          header: new sap.m.Text
         //       })

         //       ]
         //    }));
         //    oPersonalizationDialog.addContent(new sap.m.Text({
         //       text: "text"
         //    }));
         // }

      },


      //Cancel Button on Set Parameters Dialog Box
      onCancel(oEvent){
         oEvent.getSource().close();
      },

      colorPicker(oEvent) {
         this.inputId = oEvent.getSource().getId();
         if (!this.oColorPickerPopover) {
            this.oColorPickerPopover = new sap.ui.unified.ColorPickerPopover({
               colorString: "blue",
               mode: sap.ui.unified.ColorPickerMode.HSL,
               change: this.handleChange.bind(this)
            });
         }
         this.oColorPickerPopover.openBy(oEvent.getSource());
      },

      handleChange(oEvent) {
         let oView = this.getView();
         //oView.byId(this.inputId).setValue(oEvent.getParameter("colorString"));
         this.inputId = "";
         let color = oEvent.getParameter("colorString");
      },

      updateRange() {
         let data = this.getView().getModel().getData();
         let range = this.getView().byId("Slider").getRange();
         console.log("Slider " + range);

         //We pass the values from range array in JS to C++ fRange array
         data.fUpdateRange[0] = range[0];
         data.fUpdateRange[1] = range[1];
      },

   });

});
