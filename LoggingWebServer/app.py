import logging

from fastapi import FastAPI, Request
from fastapi.responses import PlainTextResponse
from datetime import datetime

app = FastAPI()

logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)

formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')


console_handler = logging.StreamHandler()
console_handler.setLevel(logging.DEBUG) # Set the minimum level for the console handler
console_handler.setFormatter(formatter)

file_handler = logging.FileHandler('app.log')
file_handler.setLevel(logging.INFO) # Set the minimum level for the file handler
file_handler.setFormatter(formatter)

logger.addHandler(console_handler)
logger.addHandler(file_handler)

logger.info("server starting up")

@app.get("/log")
async def log_data(request: Request):
    query_params = dict(request.query_params)
    fsr = query_params.get("fsr", "N/A")
    voltage = query_params.get("voltage", "N/A")
    resistance = query_params.get("resistance", "N/A")
    message = query_params.get("message", "N/A")
    
    log_entry = f"[{datetime.now().isoformat()}] FSR: {fsr}, Voltage: {voltage}, Resistance: {resistance}, Message: {message}"
    logger.info(log_entry)
    
    return PlainTextResponse(f"Logged: {log_entry}")


