import datetime

import dash
import dash_core_components as dcc
import dash_html_components as html
import plotly
import paho.mqtt.client as mqtt
import random
from dash.dependencies import Input, Output

NR_OF_CHANNELS=8

def on_message(client, userdata, message):
   global mqtt_msg
   mqtt_msg=str(message.payload.decode("utf-8"))

def on_connect(client, userdata, flags, rc):
    client.subscribe("/USB/currents/")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("localhost")

external_stylesheets = ['https://codepen.io/chriddyp/pen/bWLwgP.css']

app = dash.Dash(__name__, external_stylesheets=external_stylesheets)
app.layout = html.Div(
    html.Div([
        html.H4('Power Splitter Graphs'),
        html.Div(id='live-update-text'),
        dcc.Graph(id='graph-current'),
        dcc.Graph(id='graph-voltage'),
        dcc.Graph(id='graph-power'),
        dcc.Interval(
            id='interval-component',
            interval=1*1000, # in milliseconds
            n_intervals=0
        )
    ])
)


@app.callback(Output('live-update-text', 'children'),
              [Input('interval-component', 'n_intervals')])
def update_metrics(n):
    lon, lat, alt = random.sample(range(1, 10), 3)
    style = {'padding': '5px', 'fontSize': '16px'}
    return [
        html.Span('Longitude: {0:.2f}'.format(lon), style=style),
        html.Span('Latitude: {0:.2f}'.format(lat), style=style),
        html.Span('Altitude: {0:0.2f}'.format(alt), style=style)
    ]


# Multiple components can update everytime interval gets fired.
@app.callback(Output('live-update-graph', 'figure'),
              [Input('interval-component', 'n_intervals')])
def update_graph_live(n):
    data = {
        'time': [],
        'Latitude': [],
        'Longitude': [],
        'Altitude': []
    }

    # Collect some data
    for i in range(180):
        time = datetime.datetime.now() - datetime.timedelta(seconds=i*20)
        lon, lat, alt = random.sample(range(1, 10), 3) # satellite.get_lonlatalt(time)
        data['Longitude'].append(lon)
        data['Latitude'].append(lat)
        data['Altitude'].append(alt)
        data['time'].append(time)

    # Create the graph with subplots
    fig = plotly.tools.make_subplots(rows=2, cols=1, vertical_spacing=0.2)
    fig['layout']['margin'] = {
        'l': 30, 'r': 10, 'b': 30, 't': 10
    }
    fig['layout']['legend'] = {'x': 0, 'y': 1, 'xanchor': 'left'}

    fig.append_trace({
        'x': data['time'],
        'y': data['Altitude'],
        'name': 'Current',
        'mode': 'lines',
        'type': 'scatter'
    }, 1, 1)
    fig.append_trace({
        'x': data['time'],
        'y': data['Latitude'],
        'text': data['time'],
        'name': 'Voltage',
        'mode': 'lines',
        'type': 'scatter'
    }, 2, 1)

    return fig


if __name__ == '__main__':
    client.loop_start()
    app.run_server(debug=True, port=8050, host='0.0.0.0')

