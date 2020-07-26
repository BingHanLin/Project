import requests
from bs4 import BeautifulSoup


class ScholarQuerier():

    def __init__(self, DOI: str):

        DOISplit = DOI.split("/")

        DOIurl = 'https://scholar.google.com.tw/scholar?hl=zh-TW&as_sdt=0%2C5&q={0}%2F{1}&oq='.format(
            DOISplit[0], DOISplit[1])

        res = requests.get(DOIurl)
        soup = BeautifulSoup(res.text, "html.parser")
        print(res.text)

        # tag = soup.find(name='form', attrs={'id': 'gs_settings_form'})


if __name__ == '__main__':
    test = ScholarQuerier("10.1016/j.jcp.2007.12.006")
