import requests
from bs4 import BeautifulSoup


class ScholarQuerier():

    def __init__(self, DOI: str):

        DOISplit = DOI.split("/")

        self._DOIurl = 'https://scholar.google.com.tw/scholar?hl=zh-TW&as_sdt=0%2C5&q={0}%2F{1}&oq='.format(
            DOISplit[0], DOISplit[1])

        citesPageUrl = self.getCitesPageUrl()
        print(citesPageUrl)

    def getCitesPageUrl(self) -> str:
        res = requests.get(self._DOIurl)
        soup = BeautifulSoup(res.text, "html.parser")
        article = soup.find(id='gs_res_ccl_mid').find(
            "div", class_="gs_r gs_or gs_scl").find(
            "div", class_="gs_fl")

        links = article.find_all('a')

        return "https://scholar.google.com.tw{0}".format(links[2].get('href'))
# tag = soup.find(name='form', attrs={'id': 'gs_settings_form'})


if __name__ == '__main__':
    test = ScholarQuerier("10.1016/j.jcp.2007.12.006")
