fetch('/api/DPM')
    .then(response => response.text())  
    .then(data => {
        document.getElementById('message').innerText = data;
    })
    .catch(error => console.error('Error fetching message:', error));

    document.addEventListener('DOMContentLoaded', () => {
        const logoContainer = document.querySelector('.logo-container');
        const tf2Logo = document.querySelector('.tf2-logo');
        
        let angle = 0;           
        let rotationSpeed = 0.5; 
    
        function rotateLogo() {
            angle = (angle + rotationSpeed) % 360;
            tf2Logo.style.transform = `rotate(${angle}deg)`;
            requestAnimationFrame(rotateLogo);
        }
    
        requestAnimationFrame(rotateLogo);
    
        document.addEventListener('keydown', (event) => {
            if (event.key === 'Enter') {
                logoContainer.classList.add('zoom-effect');
                
                rotationSpeed += 1.5; 
            }
        });
    });    