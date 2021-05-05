const PORT = 8888;
const ATTEMPTS = 5;
var reconnectionAttempts;
var chart;
var first = true;
var lines = [];

function firstTimeExecuted() {
    first = false;
};

function initChart() {
    chart = new CanvasJS.Chart("chartContainer", {
        title: {
            text: ""
        },
        animationEnabled: true,
        axisX: {
            logarithmic: false,
            gridThickness: 0,
            title: "",
            lineColor: "black",
            labelFontColor: "black"
        },
        axisY: {
            logarithmic: false,
            gridThickness: 0,
            title: "",
            lineColor: "black",
            labelFontColor: "black"
        },
        legend: {
            cursor: "pointer",
            itemmouseover: function(e) {
                e.dataSeries.lineThickness = e.chart.data[e.dataSeriesIndex].lineThickness * 2;
                e.dataSeries.markerSize = e.chart.data[e.dataSeriesIndex].markerSize + 2;
                e.chart.render();
            },
            itemmouseout: function(e) {
                e.dataSeries.lineThickness = e.chart.data[e.dataSeriesIndex].lineThickness / 2;
                e.dataSeries.markerSize = e.chart.data[e.dataSeriesIndex].markerSize - 2;
                e.chart.render();
            },
            itemclick: function (e) {
                if (typeof (e.dataSeries.visible) === "undefined" || e.dataSeries.visible) {
                    e.dataSeries.visible = false;
                } else {
                    e.dataSeries.visible = true;
                }
                e.chart.render();
            }
        },
        toolTip: {
            shared: true
        },
        data: lines
    });
}

function refreshChart() {
    chart.data = lines;
    chart.render();
}

function hideElements(type) {
    var log = (type == "standard logaritmic") || (type == "logaritmic with deviation");
    var dev = (type == "with deviation") || (type == "logaritmic with deviation");
    
    if (!log) {
        document.getElementById("scale-container").style.display = "none";
    }

    if (!dev) {
        document.getElementById("deviation-container").style.display = "none";
    }
}

function storeUserPreferences(pref) {
    chart.options.axisX.title = pref.xName;
    chart.options.axisY.title = pref.yName;
    chart.options.title.text = pref.title;
    chart.render();

    hideElements(pref.type);
}

function addNewLine(line) {
    line.visible = true;
    lines.push(line);
    chart.data = lines;
}

function addRecord(parsed) {
    var lineName = parsed.name;

    delete parsed.name;
    
    var lineIndex = lines.findIndex(function(value) {
        return value.name == lineName;
    });

    lines[lineIndex].dataPoints.push(parsed);
    chart.data = lines;
}

function changeScale() {
    var log = chart.options.axisX.logarithmic;
    
    chart.options.axisX.logarithmic = !log;
    chart.options.axisY.logarithmic = !log;
    chart.data = lines;

    chart.render();
    console.log(chart.data[0].visible);
    //console.log(chart.options.axisY.logarithmic);
}

function changeDeviationVisibility() {
    var text = document.getElementById("deviation").textContent;

    if (text == "Show") {
        document.getElementById("deviation").textContent = "Hide";
    } else {
        document.getElementById("deviation").textContent = "Show";
    }

    lines.forEach(function(value) {
        if (value.name.indexOf("Deviation") != -1) {
            value.visible = !value.visible;
        }
    });

    chart.data = lines;
    chart.render();
}

function tryToReconnect(port) {
    reconnectionAttempts--;
    connect(port);
}

function resetAttempts() {
    reconnectionAttempts = ATTEMPTS;
}

function connect(port) {
    var ws = new WebSocket("ws://localhost:" + port);

    ws.onopen = function(e) {
        resetAttempts();
    }
  
    ws.onmessage = function(e) {
        var parsed = JSON.parse(e.data);

        if (first) {
            storeUserPreferences(parsed);
            firstTimeExecuted();
        } else {
            if (parsed.hasOwnProperty('type')) {
                addNewLine(parsed);
            } else {
                addRecord(parsed);
            }
    
            refreshChart();
        }
    };
  
    ws.onclose = function(e) {
        if (reconnectionAttempts > 0) {
            setTimeout(function() {
                tryToReconnect(port);
            }, 1000);
        }
    };
  
    ws.onerror = function(err) {
        console.error('Socket encountered error: ', err.message, 'Closing socket');
        ws.close();
    };
}

window.onload = function() {
    initChart();
    connect(PORT);
    initChart();
    chart.render();

    document.getElementById("scale").onclick = changeScale;
    document.getElementById("deviation").onclick = changeDeviationVisibility;
}