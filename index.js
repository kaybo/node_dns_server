const dns = require('./build/Release/convert.node');

var A_ROOT = '198.41.0.4';

//returns the ip for the given host name
const hostNameLookUp = (domainName) => {
    let resQuery = dns.query(domainName, '61.151.180.47');
    let isAnswerFound = false;
    let ansObj = undefined;
    console.log(resQuery);

    if(resQuery.answer.length === 0 && resQuery.authority.length === 0 && resQuery.additional.length === 0) return 'unknown';

    while(!isAnswerFound){
        if(resQuery.answer.length === 0){

        }else{
            //checks answer sections and handles condition of CNAME occurs
            let isAnswerIPFound = false;
            resQuery.answer.forEach(obj => {
                if(checkIfIpAddress(obj.data) === true){
                    isAnswerFound = true;
                    isAnswerIPFound = true;
                    ansObj = obj;
                };
            });
            if(!isAnswerIPFound){
                let cName = resQuery.answer[0].data;
                resQuery = dns.query(cName, A_ROOT);
            }
        }
    }

    return (resQuery.answer.length === 0) ?  'unknown' : ansObj.data;
};

const checkIfIpAddress = (data) => {
    let repetition = 0;
    for(let index = 0; index < data.length; index++){
        if(data[index] === '.') repetition++;
    }
    return (repetition === 3) ? true : false;
};


console.log(hostNameLookUp('google.com'));