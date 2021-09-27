import React, { useState, useEffect } from 'react';
import { Container, Row, Col, Button } from 'react-bootstrap';
import {  } from '@fluentui/font-icons-mdl2';
import '../styles/sequences.css';

const numberOfLEDs = 60;
const buttons = ["#141414", "#333333", "#525252", "#7a7a7a", '#FFFFFF'];

const Sequences = () => {
    const [sequence, setSequence] = useState([]);
    const [currentSequenceIndex, setCurrentSequenceIndex] = useState(0);
    const [prevLEDStripArray, setPrevLEDStripArray] = useState(undefined);
    const [LEDStripArray, setLEDStripArray] = useState(new Array(numberOfLEDs).fill('#FFFFFF'));
    const [currentColor, setCurrentColor] = useState(undefined);
    const [isPlaying, setIsPlaying] = useState(false);

    useEffect(() => {
        console.log('entro')
        if(isPlaying) {
            console.log('playing')
            setTimeout(() => {
                let newCurrentSequenceIndex = currentSequenceIndex + 1;
                if(newCurrentSequenceIndex >= sequence.length) {
                    newCurrentSequenceIndex = 0;
                }
                setCurrentSequenceIndex(newCurrentSequenceIndex);
                setLEDStripArray(sequence[newCurrentSequenceIndex]);
                setPrevLEDStripArray(newCurrentSequenceIndex === 0 ? undefined : sequence[newCurrentSequenceIndex - 1]);
            }, 300);
        }
    }, [isPlaying, currentSequenceIndex, sequence]);


    const drawLEDStrip = () => {
        if(LEDStripArray === undefined) {
            return;
        }

        return (
            LEDStripArray.map((LED, i) => (
                <div
                onClick={() => changeColor(i)} 
                style={{
                    backgroundColor:LED === '#FFFFFF' && prevLEDStripArray !== undefined  && prevLEDStripArray[i] !== '#FFFFFF' ? getPrevColor(prevLEDStripArray[i]) : LED,
                    height: '15px',
                    width: '50px',
                    border: 'solid 1px black'
                }}/>
            ))
        )
    }

    const getPrevColor = (color) => {
        const newColor = `rgba(${ parseInt(color.substring(1,3),16) }, ${ parseInt(color.substring(3,5),16) }, ${ parseInt(color.substring(5,7),16) }, 0.2)`;
        return newColor;
    }

    const changeColor = (index) => {
        if(currentColor === undefined) {
            return;
        }
        const newLEDStripArray = [ ...LEDStripArray ];
        newLEDStripArray[index] = currentColor;
        setLEDStripArray(newLEDStripArray);
    }

    const nextFrame = () => {
        const newCurrentSequenceIndex = currentSequenceIndex + 1;
        if(newCurrentSequenceIndex >= sequence.length) {
            const tempLEDStrip = LEDStripArray;
            setPrevLEDStripArray(tempLEDStrip);
            const newSequence = sequence;
            newSequence.push(tempLEDStrip);
            setSequence(newSequence);
            setLEDStripArray(new Array(numberOfLEDs).fill('#FFFFFF'));
        } else {
            const newSequence = [...sequence];
            newSequence[currentSequenceIndex] = LEDStripArray;
            setSequence(newSequence);
            setLEDStripArray(sequence[newCurrentSequenceIndex]);
            setPrevLEDStripArray(sequence[currentSequenceIndex]);
        }
        setCurrentSequenceIndex(newCurrentSequenceIndex);
    }

    const prevFrame = () => {
        const newCurrentSequenceIndex = currentSequenceIndex - 1;
        setCurrentSequenceIndex(newCurrentSequenceIndex);
        setLEDStripArray(sequence[newCurrentSequenceIndex]);
        setPrevLEDStripArray(newCurrentSequenceIndex === 0 ? undefined : sequence[newCurrentSequenceIndex - 1]);
    }

    const goToFrame = (index) => {
        setCurrentSequenceIndex(index);
        setLEDStripArray(sequence[index]);
        setPrevLEDStripArray(index === 0 ? undefined : sequence[index - 1]);
    }

    const play = () => {
        setIsPlaying(true);
    }

    const pause = () => {
        setIsPlaying(false);
    }

    return (
        <Container>
            <Row>
                <Col xs={6}>{drawLEDStrip()}</Col>
                <Col>
                    {
                        buttons.map((button, i) => (
                            <div className='row-sequence'>
                                <Button 
                                onClick={() => setCurrentColor(button)}>
                                    {button === '#FFFFFF' ? 'Erase' : `Color ${i + 1}`}
                                </Button>
                                <div 
                                className='swatch'
                                style={{backgroundColor:button}}/>
                            </div>
                        ))
                    }
                    <div className='row-sequence'>
                        {sequence.map((sequence, i) => (
                            <div className={`frame ${currentSequenceIndex === i && 'active-frame'}`}
                            onClick={() => goToFrame(i)}/>
                        ))}
                    </div>
                    <div className='row-sequence'>
                        <Button className='icons-buttons' disabled={prevLEDStripArray === undefined} onClick={() => prevFrame()}>
                            <i className="ms-Icon ms-Icon--Back"></i>
                        </Button>
                        <Button className='icons-buttons' onClick={() => nextFrame()}><i className="ms-Icon ms-Icon--Forward"></i></Button>
                        <Button className='icons-buttons' onClick={() => play()}><i className="ms-Icon ms-Icon--Play"></i></Button>
                        <Button className='icons-buttons' onClick={() => pause()}><i className="ms-Icon ms-Icon--Pause"></i></Button>
                    </div>
                </Col>
            </Row>
        </Container>
    )
}

export default Sequences;