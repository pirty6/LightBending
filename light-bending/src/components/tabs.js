import React from 'react';
import { Nav } from 'react-bootstrap';

const Tabs = () => {
    return (
        <Nav>
            <Nav.Item>
                <Nav.Link href="/">Sequences</Nav.Link>
            </Nav.Item>
            <Nav.Item>
                <Nav.Link href="/pallettes">Pallettes</Nav.Link>
            </Nav.Item>
        </Nav>
    )
}

export default Tabs;