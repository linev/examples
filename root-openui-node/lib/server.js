/**
 * The server modules is responsible to start a DEV server
 * and handle the incoming URLs to distrute them to the UI5
 * server or to handle the data requests locally.
 */
'use strict';

const express = require('express');
const proxy = require('http-proxy-middleware');
const compression = require('compression');

const app = express();

let root = {
	"name": "ROOT",
	"type": "folder",
	"nodes": [],
	"path": "/",
	"expanded": true,
	"level": 0
};


// compress all responses
app.use(compression({
	filter: function shouldCompress(req, res) {
		if (req.originalUrl.match(/^\/resources/)) {
			return false; // do not compress resources requests
		}
		// fallback to standard filter function
		return compression.filter(req, res);
	}
}));


// forward requests to resources to the UI5 dev server
app.use("/resources", proxy({target: "http://localhost:8080/", pathRewrite: {
	'^/resources/' : '/resources/'
}, changeOrigin: true}));

// serve the webapp folder locally
app.use(express.static('webapp', {
	etag: true,
	//maxAge: 12 * 60 * 60 * 1000
}));

// listen to port 3000 by default
let port = process.env.PORT || 3000;
app.listen(port, function () {
	console.log('Server listening on port ' + port + '\n');
});
