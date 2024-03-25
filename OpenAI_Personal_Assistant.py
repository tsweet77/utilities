#Requires
#   pip install openai
#   pip install SpeechRecognition
#   pip install -U gtts
#   pip install pydub
#   python -m pip install pyaudio
#   pip install ffmpeg
#   pip install -U openai==0.28
#   Add the location of ffmpeg to your system path
#   Put your OpenAI API key in a file OPEN_AI_KEY.txt

import openai
import speech_recognition as sr
from gtts import gTTS
from io import BytesIO
from pydub import AudioSegment
from pydub.playback import play

def get_openai_key():
    with open("OPEN_AI_KEY.txt", "r") as file:
        return file.read().strip()

def listen_for_trigger(recognizer, microphone):
    while True:
        with microphone as source:
            recognizer.adjust_for_ambient_noise(source)
            print("Waiting for the word 'genius'...")
            audio = recognizer.listen(source)

        try:
            text = recognizer.recognize_google(audio).lower()
            if "genius" in text:
                print("Trigger word 'genius' detected!")
                return
        except sr.UnknownValueError:
            pass

def listen_for_query(recognizer, microphone):
    with microphone as source:
        recognizer.adjust_for_ambient_noise(source)
        print("Listening for query...")
        audio = recognizer.listen(source, phrase_time_limit=5)

    try:
        text = recognizer.recognize_google(audio)
        print(f"User said: {text}")
        return text
    except sr.UnknownValueError:
        print("Speech recognition could not understand audio.")
    except sr.RequestError as e:
        print(f"Could not request results from speech recognition service; {e}")

def get_openai_response(text, api_key):
    openai.api_key = api_key
    response = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=[
            {"role": "system", "content": "You are a helpful assistant."},
            {"role": "user", "content": text}
        ]
    )
    return response.choices[0].message['content'].strip()

def text_to_speech(text):
    tts = gTTS(text, lang='en-AU')
    mp3_fp = BytesIO()
    tts.write_to_fp(mp3_fp)
    mp3_fp.seek(0)
    audio_segment = AudioSegment.from_file(mp3_fp, format="mp3")
    play(audio_segment)

def main():
    recognizer = sr.Recognizer()
    microphone = sr.Microphone()
    api_key = get_openai_key()

    while True:
        listen_for_trigger(recognizer, microphone)
        query = listen_for_query(recognizer, microphone)
        if query:
            response = get_openai_response(query, api_key)
            print(f"Assistant: {response}")
            text_to_speech(response)

if __name__ == "__main__":
    main()