# Fischertechnik Fabrik Anleitung

## API

![Lowlevel API](./low-level-api.png)

![Highlevel API](./high-level-api.png)

## MQTT

Topic | Wert | Beschreibung
--- | --- | ---
/i/wh/stock | json array | Hochregallager Belegung
/i/motor/temp | double | Motor Temperatur
/i/sl/state | text  | Sortingline Status
/i/vr/state | text | Vakuum Greifer Status
/i/wh/state | text | Hochregallager Status
/i/ps/state | text | Bearbeitungsstation Status