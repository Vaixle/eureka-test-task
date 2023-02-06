var users = require('./build/Release/users');
var express = require('express');
var router = express.Router();


/* GET home page. */
router.get('/', function(req, res, next) {
    if(req.query.username != null) {
        let response = users.get(req.query.username) === null ? "Пользователя нет" : "Пользователь есть"
        res.render('index', { title: 'User search', searchUser: response});
    } else {
        res.render('index', { title: 'User search'});
    }

});

module.exports = router;