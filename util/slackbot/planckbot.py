import os
import sys
import logging
import datetime
from collections import defaultdict

from slack import WebClient, RTMClient

import planck

logging.basicConfig(format='%(asctime)s %(levelname)s %(message)s', level=logging.INFO)
logger = logging.getLogger("slack_planck_app")
web_client = None
rtm_client = None

at_bot_id = "@UK00RUDMF"


@RTMClient.run_on(event='message')
def message_receiver(**payload):
    data = payload['data']
    client = payload['web_client']

    #logger.info(">>> .." + str(payload))

    is_bot_message = 'subtype' in data and data['subtype'] != 'bot_message'

    if not is_bot_message and 'text' in data and at_bot_id in data['text']:
        first_space_index = data['text'].find(' ')
        if first_space_index is not -1:
            channel_id = data['channel']
            code_line = data['text'][first_space_index:]

            code_line = code_line.replace("&gt;", ">")
            code_line = code_line.replace("&lt;", "<")

            st, planck_result = planck.planck_do(code_line)

            ret_text = ""
            if st == 0:
                ret_text = planck_result
            else:
                if isinstance(planck_result, str):
                    ret_text = '"' + str(planck_result) + '"'
                else:
                    if st == 3:
                        ret_text = "True" if planck_result == 1 else "False"
                    elif st == 4:
                        ret_text = "OK"
                    else:
                        ret_text = str(planck_result)

            client.chat_postMessage(
                channel=channel_id,
                text=ret_text
            )

            logger.info(">>" + code_line)
            logger.info("  " + str(planck_result))



@RTMClient.run_on(event='hello')
def hello(**payload):
    logger.info("RTM Connected")


def main():
    global web_client, rtm_client

    token = os.getenv("SLACK_TOKEN", "")
    if token == "":
        logger.error("SLACK_TOKEN is empty. Exiting.")
        sys.exit(255)

    web_client, rtm_client = init_client(token)
    if web_client is None or rtm_client is None:
        logger.error("Initialize failed. Exiting.")
        sys.exit(254)

    if not check(web_client):
        logger.error("API Check failed. Exiting.")
        sys.exit(253)

    logger.info("RTM Client start...")
    rtm_client.start()


def check(web_client) -> bool:
    resp = web_client.api_test()

    return resp.data['ok']


def init_client(token):
    web_client = WebClient(token=token)
    rtm_client = RTMClient(token=token)

    return web_client, rtm_client


if __name__ == '__main__':
    main()
