const dns = require('./build/Release/convert.node');

var A_ROOT = '198.41.0.4';

//returns the ip for the given host name
const hostNameLookUp = (domainName) => {
    let tempDomainName = domainName;
    let resQuery = dns.query(tempDomainName, A_ROOT);
    let isAnswerFound = false;
    let ansObj = undefined;
    let flag = false;

    if(resQuery.answer.length === 0 && resQuery.authority.length === 0 && resQuery.additional.length === 0) return 'unknown';

    while(!isAnswerFound){
        if(resQuery.answer.length === 0){
            //takes the first RR of the additional section and uses that ip to do iterative lookup
            if(resQuery.authority.length > 0 && resQuery.additional.length > 0){
                let newIpAddress = resQuery.additional[0].data;
                resQuery = dns.query(tempDomainName, newIpAddress);
            }else{
                if(resQuery.authority.length > 0){
                    flag = true;
                    tempDomainName = resQuery.authority[0].data;
                    resQuery = dns.query(tempDomainName, A_ROOT);
                }else{
                    return 'unknown';
                }
            }
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
                tempDomainName = resQuery.answer[0].data;
                resQuery = dns.query(tempDomainName, A_ROOT);
            }
            if(flag){
                tempDomainName = domainName;
                resQuery = dns.query(tempDomainName, ansObj.data);
                flag = false;
                isAnswerFound = false;
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


module.exports = {
    hostNameLookUp
};