# the-root-project

## Overview

Run ROOT openui-based widget from node.js

## Getting started

To get started just clone this project and run:

```bash
npm i
```

Once the dependencies are downloaded you can run the server with:

```bash
npm start
```

To run the sample in the browser use the following URL:

```
http://localhost:3000
```

## Insights

Checkout the code inside of:

* ```webapp/model/Model.js```
* ```webapp/model/ListBinding.js```

The Model takes care about loading the data from the server and the ListBinding is the connection to the TreeTable control. Currently the ListBinding is only implemented to fetch the data from the server and to handle expand and collapse. Not more. The implementation is not really smart yet and with more time this complete loading behavior can be optimized in a much better fashion. But it at least sketches the basic idea of using the Model and the ListBinding as just a delegation to a server to load just an excerpt of the data needed for the client.
