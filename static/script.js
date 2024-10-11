fetch('/api/DPM')
    .then(response => response.text())  
    .then(data => {
        document.getElementById('message').innerText = data;
    })
    .catch(error => console.error('Error fetching message:', error));
