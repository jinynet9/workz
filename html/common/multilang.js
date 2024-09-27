//언어전환 기능 관련 function 집합
var languageFile = new XMLHttpRequest();
function languageFileLoad(fileName){
    languageFile.open("GET", fileName, false);
    languageFile.onreadystatechange = function languageReadResponse(){
        if(languageFile.readyState === 4) {
            if(languageFile.status === 200 || languageFile.status == 0) {
                var allText = languageFile.responseText;
                //alert(allText);

                languageApply(allText);
            }
        }
    };
    languageFile.send(null);
}

function languageApply(jsonText){
    //var xml2json = new X2JS();
    //var jsonObj = xml2json.xml_str2json(xmltext);
    var data = JSON.parse(jsonText);

    i18next.init({
        lng: 'kr',
        resources: data.template
    }, function(err, t) {
        jqueryI18next.init(i18next, $, {
            i18nName: 'i18next'
        });

        $('body').localize();
    });
}

function languageChange(language) {
	if(!language){
		return;
	}
	
	var tempLang = language;
	if(language == 'ko'){
		tempLang = 'kr'
	}
	
	i18next.changeLanguage(tempLang);
    $('body').localize();    
}
