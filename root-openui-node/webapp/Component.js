sap.ui.define([
   'sap/ui/core/UIComponent', 'sap/ui/core/mvc/XMLView', 'sap/m/routing/Router' /* Router is only needed for packaging */
], function(UIComponent, XMLView) {
   "use strict";

   var Component = UIComponent.extend("eve.Component", {
      metadata : {
         manifest: "json"
      },
      init: function() {
         UIComponent.prototype.init.apply(this, arguments);
         this.getRouter().initialize();
      },
      createContent1: function() {
         var kind = JSROOT.GetUrlOption('view');

         if (!kind) {
            return sap.ui.xmlview({
                id: "TopEveId",
                viewName: "eve.Main"
            });
         } else {
            return sap.ui.xmlview({
               id: "TopGlId",
               viewName: "eve.GL",
               viewData: { conn_handle: this.getComponentData().conn_handle, standalone: kind }
           });
         }
      }
   });

   return Component;

});
